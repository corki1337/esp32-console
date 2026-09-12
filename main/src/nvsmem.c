#include "nvsmem.h"
#include "esp_err.h"
#include <string.h>
#include "nvs_flash.h"
#include "nvs.h"


void nvs_init(void){
    esp_err_t err = nvs_flash_init();
    if(err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND){
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);
}
void nvs_save(const char* game_name, const char* variable_name, uint16_t variable){

    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(game_name, NVS_READWRITE, &my_handle);
    if (err != ESP_OK){
        return;
    }

    uint16_t current_value = UINT16_MAX;
    nvs_get_u16(my_handle, variable_name, &current_value);


    if(current_value != variable){
        err = nvs_set_u16(my_handle, variable_name, variable);


        if (err != ESP_OK){
            nvs_close(my_handle);
            return;
        }
        nvs_commit(my_handle);
    }
    nvs_close(my_handle);
}


uint16_t nvs_read(const char* game_name, const char* variable_name){
    nvs_handle_t my_handle;
    esp_err_t err = nvs_open(game_name, NVS_READONLY, &my_handle);
    if (err != ESP_OK){
        return UINT16_MAX;
    }
    uint16_t variable;
    err = nvs_get_u16(my_handle, variable_name, &variable);
    nvs_close(my_handle);
    if(err == ESP_OK){
        return variable;
    }else{
        return UINT16_MAX;
    }
}