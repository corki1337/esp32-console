#include "audio.h"
#include "i2s.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "string.h"


#define AUDIO_CHUNK 256

typedef struct {
    const int16_t *data;
    size_t total_samples;
    size_t current_sample;
    float volume;
    bool is_playing;
} audio_state_t;

static audio_state_t s_audio = {0};
static SemaphoreHandle_t s_audio_mutex = NULL;

static void audio_task(void *pvParameters){
    int16_t chunk[AUDIO_CHUNK];
    size_t bytes_written;

    while(1){
        bool has_work = false;
        const int16_t *src = NULL;
        size_t to_process = 0;
        float vol = 1.f;

        if(xSemaphoreTake(s_audio_mutex, portMAX_DELAY) == pdTRUE){
            if(s_audio.is_playing && s_audio.current_sample < s_audio.total_samples){

                has_work = true;
                src = &s_audio.data[s_audio.current_sample];

                size_t remaining = s_audio.total_samples - s_audio.current_sample;
                to_process = (remaining > AUDIO_CHUNK) ? AUDIO_CHUNK : remaining;
                vol = s_audio.volume;

                s_audio.current_sample += to_process;
                if(s_audio.current_sample >= s_audio.total_samples){
                    s_audio.is_playing = false;
                }
            }
            xSemaphoreGive(s_audio_mutex);
        }
        if(has_work){
            if(vol == 1.f){
                memcpy(chunk, src, to_process * sizeof(int16_t));
            }else{
                for(int i = 0; i < to_process; i++){
                    chunk[i] = (int16_t)(src[i] * vol);
                }
            }
            if(to_process < AUDIO_CHUNK){
                memset(&chunk[to_process], 0, (AUDIO_CHUNK - to_process) * sizeof(int16_t));
            }
            i2s_channel_write(tx_handle, chunk, sizeof(int16_t) * AUDIO_CHUNK, &bytes_written, portMAX_DELAY);
        }else{
            int16_t silence[AUDIO_CHUNK] = {0};
            size_t bw;
            i2s_channel_write(tx_handle, silence, sizeof(silence), &bw, 0);
            vTaskDelay(pdMS_TO_TICKS(10));
        }
    }
}





void audio_init(void){
    if(s_audio_mutex == NULL){
        s_audio_mutex = xSemaphoreCreateMutex();
        xTaskCreate(audio_task, "audio_task", 3072, NULL, 3, NULL);
    }
}

void audio_play(const int16_t *data, size_t size, float volume){

    if(s_audio_mutex == NULL) return;

    if(xSemaphoreTake(s_audio_mutex, pdMS_TO_TICKS(5)) == pdTRUE){
        s_audio.data = data;
        s_audio.total_samples = size;
        s_audio.current_sample = 0;
        s_audio.volume = volume;
        s_audio.is_playing = true;
        xSemaphoreGive(s_audio_mutex);
    }
}