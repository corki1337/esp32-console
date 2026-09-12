#include "Settings.hpp"
#include "sprites.hpp"


extern "C"{
    #include "adc.h"
    #include "battery.h"
    #include "nvsmem.h"
}


void Settings::init_buttons(){
    buttons[0] = {
        .x = 30,
        .y = 10,
        .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT,
        .sprites = {mutebutton1, mutebutton2}
    };
}

void Settings::init_sliders(){
    uint16_t volume_level = nvs_read("Settings", "volume");
    uint16_t brighntess_level = nvs_read("Settings", "brightness");
    volume_level = (volume_level > 96) ? 96 : volume_level;
    brighntess_level = (brighntess_level > 96) ? 96 : brighntess_level;
    sliders[0] = {
        .x = 30,
        .y = 40, 
        .width = SLIDER_WIDTH,
        .height = SLIDER_HEIGHT,
        .progress = volume_level,
        .slidersprite = plainslider,
        .textwidth = 62,
        .textheight = 10,
        .textsprite = volume
    };
    sliders[1] = {
        .x = 30,
        .y = 70, 
        .width = SLIDER_WIDTH,
        .height = SLIDER_HEIGHT,
        .progress = brighntess_level,
        .slidersprite = plainslider,
        .textwidth = 90,
        .textheight = 10,
        .textsprite = brightness
    };

}


void Settings::draw_battery_level(Renderer *renderer){

    uint8_t battery_perc = get_battery_percentage(battery_voltage);
    renderer->drawRect(134, 8, 22, 14, BLACK);
    renderer->drawRect(132, 11, 2, 7, BLACK);
    renderer->drawRect(156, 11, 2, 7, BLACK);
    renderer->drawRect(135, 10, 20, 9, WHITE);
    renderer->drawRect(135, 10, battery_perc/5, 9, (battery_perc > 20) ? GREEN : RED);

    renderer->drawNumber(137, 11, battery_perc, BLACK);
}

void Settings::init(){
    init_buttons();
    init_sliders();
    is_muted = nvs_read("Settings", "ismuted");
    is_muted = (is_muted >= 1) ? 1 : 0;
    actbutton = 0;
    battery_voltage = adc_read() * 2;
    return;
}

void Settings::draw_buttons(Renderer* renderer){
    for(int i = 0; i < BUTTON_COUNT; i++){
        renderer->drawSprite(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, buttons[i].sprites[(i == actbutton) ? 1 : 0]);
    }
    renderer->drawSprite(buttons[0].x + 70, buttons[0].y + 3, 20, 13, (is_muted) ? muted : unmuted);
}

void Settings::draw_sliders(Renderer *renderer){
    for(int i = 0; i < SLIDER_COUNT; i++){
        renderer->drawRect(sliders[i].x + 2, sliders[i].y + 2, sliders[i].progress, sliders[i].height - 4, (actbutton == i + 1) ? LIGHT_GREEN : GREEN);
        renderer->drawRect(sliders[i].x + 2 + sliders[i].progress, sliders[i].y + 2, sliders[i].width - sliders[i].progress - 4, sliders[i].height - 4, WHITE);
        renderer->drawSprite(sliders[i].x, sliders[i].y, sliders[i].width, sliders[i].height, sliders[i].slidersprite);
        renderer->drawSprite(sliders[i].x + (sliders[i].width - sliders[i].textwidth)/2, sliders[i].y + (sliders[i].height - sliders[i].textheight)/2, sliders[i].textwidth, sliders[i].textheight, sliders[i].textsprite);
    }
}

GameState Settings::get_game_state(){
    return state;
}


void Settings::update_battery(){
    if(battery_tick >=  60 * 5){
        battery_tick = 0;
        battery_voltage = adc_read() * 2;
    }
    battery_tick++;   
}

ChosenGame Settings::update(GameInput input, SoundPlayer* soundplayer){
    update_battery();

    if(input_limiter > INPUT_LIMIT){
        switch(actbutton){
            case 0:
                if(input.action){
                    is_muted = (is_muted >=1) ? 0 : 1;
                    soundplayer->setmute((is_muted == 1) ? true : false);
                    input_limiter = 0;
                }else if(input.down){
                    actbutton = 1;
                    input_limiter = 0;
                }
                break;
            case 1:
                if(input.right && sliders[0].progress <= 94){
                    sliders[0].progress += 2;
                    
                    soundplayer->setVolume((float)(sliders[0].progress) / 96.0f);
                    input_limiter = INPUT_LIMIT * 3/4;
                }else if(input.left && sliders[0].progress > 2){
                    sliders[0].progress -= 2;
                    float vol = (float)(sliders[0].progress) / 96.0f;
                    soundplayer->setVolume(vol);
                    input_limiter = INPUT_LIMIT *3/4;
                }else if(input.up){
                    actbutton = 0;
                    input_limiter = 0;
                }else if(input.down){
                    actbutton = 2;
                    input_limiter = 0;
                }
                break;
            case 2:
                if(input.right && sliders[1].progress <= 94){
                    sliders[1].progress += 2;
                    lcd_set_brightness((sliders[1].progress > 15) ? sliders[1].progress : 15);
                    input_limiter = INPUT_LIMIT / 2;
                }else if(input.left && sliders[1].progress >= 2){
                    sliders[1].progress -= 2;
                    lcd_set_brightness((sliders[1].progress > 15) ? sliders[1].progress : 15);
                    input_limiter = INPUT_LIMIT / 2;
                }else if(input.up){
                    actbutton = 1;
                    input_limiter = 0;
                }
                break;

            
        }
    }


    if(input.back){
        nvs_save("Settings", "volume", sliders[0].progress);
        nvs_save("Settings", "brightness", sliders[1].progress);
        nvs_save("Settings", "ismuted", is_muted);
        return ChosenGame::MENU;
    }

    if(input_limiter <= INPUT_LIMIT) input_limiter++;
    return ChosenGame::SETTINGS;
}


void Settings::draw(Renderer* renderer){

    renderer->drawBackground(menubg);
    draw_buttons(renderer);
    draw_battery_level(renderer);
    draw_sliders(renderer);


    renderer->display();
}