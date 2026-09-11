#include "gpio.h"


void gpio_init(void){
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << LCD_DC) | (1ULL << LCD_RST),
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);

    gpio_config_t io_conf2 = {
        .pin_bit_mask = (1ULL << ACTION_BUTTON) | (1ULL << DOWN_BUTTON)| (1ULL << UP_BUTTON)| (1ULL << RIGHT_BUTTON)| (1ULL << LEFT_BUTTON)| (1ULL << BACK_BUTTON),
        .mode = GPIO_MODE_INPUT,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf2);
}
