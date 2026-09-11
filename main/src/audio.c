#include "audio.h"
#include "i2s.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


static volatile uint8_t is_transmission_done = 1;

void send_done(void){
    is_transmission_done = 1;
}


void audio_play(const int16_t *data, size_t size){

    //if(!is_transmission_done) return;

    is_transmission_done = 0;
    size_t bytes_written = 0;
    i2s_channel_write(tx_handle, data, size, &bytes_written, portMAX_DELAY);

    int16_t silence[512] = {0}; 
    i2s_channel_write(tx_handle, silence, sizeof(silence), &bytes_written, portMAX_DELAY);
}