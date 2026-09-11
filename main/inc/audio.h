#pragma once
#include "stdint.h"



#ifdef __cplusplus
extern "C" {
#endif

void audio_init(void);

void audio_play(const int16_t *data, size_t size, float volume);


#ifdef __cplusplus
}
#endif