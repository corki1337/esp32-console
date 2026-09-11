#pragma once

#include "driver/gpio.h"

#define LCD_DC 48
#define LCD_RST 21
#define ACTION_BUTTON 4
#define DOWN_BUTTON 5
#define UP_BUTTON 6
#define RIGHT_BUTTON 7
#define LEFT_BUTTON 15
#define BACK_BUTTON 16

#ifdef __cplusplus
extern "C" {
#endif


void gpio_init(void);

#ifdef __cplusplus
}
#endif