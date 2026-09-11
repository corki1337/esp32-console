idf_component_register(
    SRC_DIRS "." "src"
    INCLUDE_DIRS "." "inc"
    REQUIRES driver esp_driver_spi esp_driver_gpio nvs_flash  esp_driver_i2s esp_driver_ledc esp_adc
)