#include "i2s.h"
#include "esp_attr.h"
#include "audio.h"




i2s_chan_handle_t tx_handle;


static bool IRAM_ATTR my_i2s_callback(i2s_chan_handle_t handle, i2s_event_data_t *event, void *user_ctx) {

    send_done();
    return false;
}

void i2s_init(void){

    i2s_chan_config_t chan_cfg = I2S_CHANNEL_DEFAULT_CONFIG(I2S_NUM_0, I2S_ROLE_MASTER);
    chan_cfg.auto_clear = true;
    esp_err_t ret = i2s_new_channel(&chan_cfg, &tx_handle, NULL);
    ESP_ERROR_CHECK(ret);

    i2s_event_callbacks_t cbs = {
        .on_sent = my_i2s_callback
    };
    i2s_channel_register_event_callback(tx_handle, &cbs, NULL);

    i2s_std_config_t std_cfg = {
        .clk_cfg = I2S_STD_CLK_DEFAULT_CONFIG(16000),
        .slot_cfg = I2S_STD_MSB_SLOT_DEFAULT_CONFIG(I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO),
        .gpio_cfg = {
            .mclk = I2S_GPIO_UNUSED,
            .bclk = (gpio_num_t)I2S_BCLK_PIN,
            .ws = (gpio_num_t)I2S_LRC_PIN,
            .dout = (gpio_num_t)I2S_DIN_PIN,
            .din = I2S_GPIO_UNUSED
        }
    };

    ret = i2s_channel_init_std_mode(tx_handle, &std_cfg);
    ESP_ERROR_CHECK(ret);

    ret = i2s_channel_enable(tx_handle);
    ESP_ERROR_CHECK(ret);


}