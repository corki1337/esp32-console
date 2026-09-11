#include "spi.h"
#include "lcd.h"
#include "esp_attr.h"
spi_device_handle_t spi_handle;

void IRAM_ATTR my_spi_tx_cplt_callback(spi_transaction_t *t) {
    lcd_copy_done(t);
}
void spi_init(void){
    
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = PIN_NUM_CLK,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
        .max_transfer_sz = 81920 
    };

    spi_device_interface_config_t dev_cfg = {
        .clock_speed_hz = 15 * 1000 * 1000,
        .mode = 0,
        .spics_io_num = LCD_CS,
        .queue_size = 7,
        .post_cb = my_spi_tx_cplt_callback

    }; 

    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
    ESP_ERROR_CHECK(ret);

    ret = spi_bus_add_device(SPI2_HOST, &dev_cfg, &spi_handle);
    ESP_ERROR_CHECK(ret);
}