
extern "C" {

    #include <stdio.h>
    #include "spi.h"
    #include "gpio.h"
    #include "lcd.h"
    #include "freertos/FreeRTOS.h"
    #include "freertos/task.h"
    #include <stdio.h>
    #include "nvsmem.h"
    #include "i2s.h"
    #include "audio.h"
    #include "pwm.h"
    #include "adc.h"
}

#include "Renderer.hpp"
#include "SoundPlayer.hpp"
#include "DinoGame.hpp"
#include "BirdGame.hpp"
#include "Menu.hpp"
#include "sounds.hpp"
#include "Settings.hpp"


static const uint8_t INPUT_LIMIT = 15;



extern "C" void app_main(void)
{   
    gpio_init();
    pwm_init();
    spi_init();
    i2s_init();
    audio_init();
    nvs_init();
    adc_init();


    Renderer renderer;
    renderer.init();

    IGame* game = new Menu();
    ChosenGame actual_game = ChosenGame::MENU;

    game->init();
    game->draw(&renderer);
    

    //vTaskDelay(200);


    SoundPlayer* soundplayer = new SoundPlayer();



    //soundplayer->playSound((int16_t*)hitHurt_2_, sizeof(hitHurt_2_));

    //vTaskDelay(pdMS_TO_TICKS(500));

    //soundplayer->playSound((int16_t*)powerUp, sizeof(powerUp));


    uint8_t input_limiter = 0;



    vTaskDelay(pdMS_TO_TICKS(200));



    while(1){
        GameInput input = {};



        if(input_limiter >= INPUT_LIMIT){
            input.action = !(bool)gpio_get_level(GPIO_NUM_4);
            input.down = !(bool)gpio_get_level(GPIO_NUM_5);
            input.up = !(bool)gpio_get_level(GPIO_NUM_6);
            input.right = !(bool)gpio_get_level(GPIO_NUM_7);
            input.left = !(bool)gpio_get_level(GPIO_NUM_15);
            input.back = !(bool)gpio_get_level(GPIO_NUM_16);
        }else{
            input_limiter++;
        }




        ChosenGame choice = game->update(input, soundplayer);
        game->draw(&renderer);
        


        if(choice != actual_game){
            delete game;
            input_limiter = 0;
            switch(choice){
                case ChosenGame::DINO_GAME:
                    game = new DinoGame();
                    break;
                case ChosenGame::BIRD_GAME:
                    game = new BirdGame();
                    break;
                case ChosenGame::MENU:
                    game = new Menu();
                    break;
                case ChosenGame::SETTINGS:
                    game = new Settings();
                    break;
                default:
                    break;
            }
            actual_game = choice;
            game->init();
        }
        
        vTaskDelay(pdMS_TO_TICKS(16));


    }
}
