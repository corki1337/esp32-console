#pragma once
#include "driver/spi_master.h"




// PIN CONFIG
#define PIN_NUM_MOSI 11  
#define PIN_NUM_CLK  12
#define LCD_CS 47


extern spi_device_handle_t spi_handle;

#ifdef __cplusplus
extern "C" {
#endif

// initialization function
void spi_init(void);


#ifdef __cplusplus
}
#endif