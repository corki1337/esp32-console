#pragma once
#include "stdint.h"



#ifdef __cplusplus
extern "C" {
#endif

void send_done(void);

void audio_play(const int16_t *data, size_t size);


#ifdef __cplusplus
}
#endif