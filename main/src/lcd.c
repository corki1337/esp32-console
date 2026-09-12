#include "lcd.h"
#include "gpio.h"
#include "spi.h"
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "pwm.h"
#include "math.h"
#include "nvsmem.h"

#define ST7735S_SLPOUT			0x11
#define ST7735S_DISPOFF			0x28
#define ST7735S_DISPON			0x29
#define ST7735S_CASET			0x2a
#define ST7735S_RASET			0x2b
#define ST7735S_RAMWR			0x2c
#define ST7735S_MADCTL			0x36
#define ST7735S_COLMOD			0x3a
#define ST7735S_FRMCTR1			0xb1
#define ST7735S_FRMCTR2			0xb2
#define ST7735S_FRMCTR3			0xb3
#define ST7735S_INVCTR			0xb4
#define ST7735S_PWCTR1			0xc0
#define ST7735S_PWCTR2			0xc1
#define ST7735S_PWCTR3			0xc2
#define ST7735S_PWCTR4			0xc3
#define ST7735S_PWCTR5			0xc4
#define ST7735S_VMCTR1			0xc5
#define ST7735S_GAMCTRP1		0xe0
#define ST7735S_GAMCTRN1		0xe1
#define LCD_OFFSET_X  1
#define LCD_OFFSET_Y  2

#define CMD(x)			((x) | 0x100)

static volatile uint8_t lcd_ready = 1;


#define TX_BUF_SIZE LCD_WIDTH * LCD_HEIGHT
static uint16_t frame_buffer[TX_BUF_SIZE * 2];
static uint16_t *draw_buf = &frame_buffer[0];
static uint16_t *send_buf = &frame_buffer[TX_BUF_SIZE];



static void lcd_cmd(uint8_t cmd){

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &cmd;
    t.rx_buffer = NULL;


    gpio_set_level(LCD_DC, 0);
    spi_device_polling_transmit(spi_handle, &t);
}

static void lcd_data(uint8_t data){

    spi_transaction_t t;
    memset(&t, 0, sizeof(t));
    t.length = 8;
    t.tx_buffer = &data;
    t.rx_buffer = NULL;

    gpio_set_level(LCD_DC, 1);
    spi_device_polling_transmit(spi_handle, &t);
}



static void lcd_send(uint16_t value){
    if(value & 0x100){
        lcd_cmd(value);
    }else{
        lcd_data(value);
    }
}


static const uint16_t init_table[] = {
  CMD(ST7735S_FRMCTR1), 0x01, 0x2c, 0x2d,
  CMD(ST7735S_FRMCTR2), 0x01, 0x2c, 0x2d,
  CMD(ST7735S_FRMCTR3), 0x01, 0x2c, 0x2d, 0x01, 0x2c, 0x2d,
  CMD(ST7735S_INVCTR), 0x07,
  CMD(ST7735S_PWCTR1), 0xa2, 0x02, 0x84,
  CMD(ST7735S_PWCTR2), 0xc5,
  CMD(ST7735S_PWCTR3), 0x0a, 0x00,
  CMD(ST7735S_PWCTR4), 0x8a, 0x2a,
  CMD(ST7735S_PWCTR5), 0x8a, 0xee,
  CMD(ST7735S_VMCTR1), 0x0e,
  CMD(ST7735S_GAMCTRP1), 0x0f, 0x1a, 0x0f, 0x18, 0x2f, 0x28, 0x20, 0x22,
                         0x1f, 0x1b, 0x23, 0x37, 0x00, 0x07, 0x02, 0x10,
  CMD(ST7735S_GAMCTRN1), 0x0f, 0x1b, 0x0f, 0x17, 0x33, 0x2c, 0x29, 0x2e,
                         0x30, 0x30, 0x39, 0x3f, 0x00, 0x07, 0x03, 0x10,
  CMD(0xf0), 0x01,
  CMD(0xf6), 0x00,
  CMD(ST7735S_COLMOD), 0x05,
  CMD(ST7735S_MADCTL), 0x60,
};

void lcd_init(void){

    int i;

    gpio_set_level(LCD_RST, 0);
    vTaskDelay(pdMS_TO_TICKS(100));
    gpio_set_level(LCD_RST, 1);
    vTaskDelay(pdMS_TO_TICKS(100));

    for(i = 0; i < sizeof(init_table)/sizeof(uint16_t); i++){
        lcd_send(init_table[i]);
    }
    vTaskDelay(pdMS_TO_TICKS(200));

    lcd_cmd(ST7735S_SLPOUT);
    vTaskDelay(pdMS_TO_TICKS(120));

    lcd_cmd(ST7735S_DISPON);

    lcd_set_brightness(((nvs_read("Settings", "brightness") > 15) ? nvs_read("Settings", "brightness"): 15)); 
}

void IRAM_ATTR lcd_copy_done(spi_transaction_t *t){
    if ((int)t->user ==2){
        lcd_ready = 1;
    }
    
}
 uint8_t lcd_is_ready(void){
    return lcd_ready;
 }


static void lcd_data16(uint16_t value){
    lcd_data((uint8_t)(value>>8));
    lcd_data((uint8_t)(value & 0xFF));
}

static void lcd_set_window(int x, int y, int width, int height){
    lcd_cmd(ST7735S_CASET);
    lcd_data16(LCD_OFFSET_X + x);
    lcd_data16(LCD_OFFSET_X + x + width -1);

    lcd_cmd(ST7735S_RASET);
    lcd_data16(LCD_OFFSET_Y + y);
    lcd_data16(LCD_OFFSET_Y + y + height -1);
}





void lcd_put_pixel(int x, int y, uint16_t color) {
    if (x >= 0 && x < LCD_WIDTH && y >= 0 && y < LCD_HEIGHT) {
        draw_buf[x + y * LCD_WIDTH] = color;
    }
}


uint16_t* lcd_get_draw_buffer(void){
    return draw_buf;
}

void lcd_clear_screen(uint16_t color){
    if(color == BLACK){
        memset(draw_buf, 0,sizeof(uint16_t) * TX_BUF_SIZE);
    }else{
        for(int i = 0; i < TX_BUF_SIZE; i++){
            draw_buf[i] = color;
        }
    }
}

void lcd_set_brightness(uint16_t percentage){
    if(percentage > 100){
        return;
    }
    float brightness_corected = 100.0f * pow((float)percentage / 100.0f, 2.5f);
    pwm_percent_write((uint16_t)(brightness_corected + 0.5f));
}


void lcd_copy(void){

    while(!lcd_ready){
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    uint16_t *temp = draw_buf;
    draw_buf = send_buf;
    send_buf = temp;
    
    lcd_ready = 0;


    lcd_set_window(0, 0, LCD_WIDTH, LCD_HEIGHT);
    lcd_cmd(ST7735S_RAMWR);

    gpio_set_level(LCD_DC, 1);

    static spi_transaction_t t1 = {0};
    static spi_transaction_t t2 = {0};
    //first half 
    t1.length = LCD_WIDTH * LCD_HEIGHT * 8; 
    t1.tx_buffer = send_buf; 
    t1.rx_buffer = NULL;
    t1.user = (void*)1;
    //second
    t2.length = LCD_WIDTH * LCD_HEIGHT * 8;
    t2.tx_buffer = &send_buf[(LCD_HEIGHT/2) * LCD_WIDTH]; 
    t2.rx_buffer = NULL;
    t2.user = (void*)2;



    spi_device_queue_trans(spi_handle, &t1, portMAX_DELAY);
    spi_device_queue_trans(spi_handle, &t2, portMAX_DELAY);


}