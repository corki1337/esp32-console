#pragma once

#include <stdint.h>
#include "driver/spi_master.h"



#define LCD_WIDTH	160
#define LCD_HEIGHT	128



#define BLACK			0x0000
#define RED		    	0x00f8
#define GREEN			0xe007
#define BLUE			0x1f00
#define YELLOW			0xe0ff
#define CYAN			0xff07
#define WHITE			0xffff


#ifdef __cplusplus
extern "C" {
#endif

// screen init
void lcd_init(void); 

// sets flag lcd_ready to true
void lcd_copy_done(spi_transaction_t *t); 

// checks if lcd transmission is done
uint8_t lcd_is_ready(void); 

// puts pixel on draw buffer
void lcd_put_pixel(int x, int y, uint16_t color);

// returns pointer to draw buffer
uint16_t* lcd_get_draw_buffer(void);

// clears screen buffer
void lcd_clear_screen(uint16_t color);

// sets brigthness of screen in percetage (0-100)
void lcd_set_brightness(uint16_t percentage);

// buffer sender
void lcd_copy(void); 


#ifdef __cplusplus
}
#endif