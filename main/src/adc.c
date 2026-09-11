#include "adc.h"
#include "esp_attr.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"




adc_oneshot_unit_handle_t adc1_handle;
adc_cali_handle_t cali_handle;

void adc_init(void){
    adc_oneshot_unit_init_cfg_t adc1_init_config = {
        .unit_id = ADC_UNIT_1
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&adc1_init_config, &adc1_handle));
    
    adc_oneshot_chan_cfg_t adc_config = {
        .bitwidth = ADC_BITWIDTH_DEFAULT,
        .atten = ADC_ATTEN_DB_12
    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc1_handle, ADC_CHANNEL_0, &adc_config));

    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = ADC_UNIT_1,
        .chan = ADC_CHANNEL_0,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };

    ESP_ERROR_CHECK(adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle));
    
}

int adc_read(void){
    int raw_val = 0;
    int voltage_mv = 0;

    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL_0, &raw_val));

    ESP_ERROR_CHECK(adc_cali_raw_to_voltage(cali_handle, raw_val, &voltage_mv));

    return voltage_mv;
}