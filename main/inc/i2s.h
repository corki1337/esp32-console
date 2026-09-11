#pragma once
#include "driver/i2s_std.h"

#define I2S_LRC_PIN 13
#define I2S_BCLK_PIN 14
#define I2S_DIN_PIN 10

extern i2s_chan_handle_t tx_handle;


#ifdef __cplusplus
extern "C" {
#endif

void i2s_init(void);


#ifdef __cplusplus
}
#endif