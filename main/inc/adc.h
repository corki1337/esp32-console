#pragma once
#include "esp_adc/adc_oneshot.h"

extern adc_oneshot_unit_handle_t adc1_handle;

void adc_init(void);

int adc_read(void);