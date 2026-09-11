#pragma once

#include "driver/ledc.h"

#define LCD_BL_PIN 17


// inits pwm
void pwm_init(void);

// sets pulse width 
void pwm_percent_write(uint16_t percent);