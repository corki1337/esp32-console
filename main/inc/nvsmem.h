#pragma once

#include <stdint.h>
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"


#ifdef __cplusplus
extern "C" {
#endif


// inits non-volatile storage
void nvs_init(void);

// saves score to non-volatile storage
void nvs_save(const char* game_name, const char* variable_name, uint16_t variable);

// reads score from non-volatile storage
uint16_t nvs_read(const char* game_name, const char* variable_name);


#ifdef __cplusplus
}
#endif