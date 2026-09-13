#include "Menu.hpp"
#include "sprites.hpp"

extern "C"{
    #include "adc.h"
    #include "battery.h"
}


void Menu::init_buttons(){
    buttons[0] = {
        .x = 30,
        .y = 10,
        .width =  BUTTON_WIDTH,
        .height = BUTTON_HEIGHT,
        .sprites = {dinobutton1, dinobutton2},
    };
    buttons[1] = {
        .x = 30,
        .y = 40,
        .width =  BUTTON_WIDTH,
        .height = BUTTON_HEIGHT,
        .sprites = {birdbutton1, birdbutton2},
    };
    buttons[2] = {
        .x = 30,
        .y = 70,
        .width =  BUTTON_WIDTH,
        .height = BUTTON_HEIGHT,
        .sprites = {snakebutton1, snakebutton2},
    };
    buttons[3] = {
        .x = 30,
        .y = 100,
        .width =  BUTTON_WIDTH,
        .height = BUTTON_HEIGHT,
        .sprites = {settingsbutton1, settingsbutton2},
    };
}

void Menu::draw_battery_level(Renderer *renderer){

    uint8_t battery_perc = get_battery_percentage(battery_voltage);
    renderer->drawRect(134, 8, 22, 14, BLACK);
    renderer->drawRect(132, 11, 2, 7, BLACK);
    renderer->drawRect(156, 11, 2, 7, BLACK);
    renderer->drawRect(135, 10, 20, 9, WHITE);
    renderer->drawRect(135, 10, battery_perc/5, 9, (battery_perc > 20) ? GREEN : RED);

    renderer->drawNumber(137, 11, battery_perc, BLACK);
}

void Menu::draw_buttons(Renderer* renderer){
    for(int i = 0; i < BUTTON_COUNT; i++){
        renderer->drawSprite(buttons[i].x, buttons[i].y, buttons[i].width, buttons[i].height, buttons[i].sprites[(i == actbutton) ? 1 : 0]);
    }

}

void Menu::init(){
    init_buttons();
    actbutton = 0;
    battery_voltage = adc_read() * 2;
    return;
}

void Menu::update_battery(){
    if(battery_tick >=  60 * 5){
        battery_tick = 0;
        battery_voltage = adc_read() * 2;
    }
    battery_tick++;    
}

ChosenGame Menu::update(GameInput input, SoundPlayer* soundplayer){

    update_battery();

    if(input.action){
        switch(actbutton){
            case 0:
                return ChosenGame::DINO_GAME;
            case 1:
                return ChosenGame::BIRD_GAME;
            case 2:
                return ChosenGame::SNAKE_GAME;
            case 3:
                return ChosenGame::SETTINGS;
        }
    }

    if(input_limiter > INPUT_LIMIT){
        if(input.up && actbutton > 0){
            actbutton--;
            input_limiter = 0;
        }else if(input.down && actbutton < 3){
            actbutton++;
            input_limiter = 0;
        }
    }
    if(input_limiter <= INPUT_LIMIT) input_limiter++;
    return ChosenGame::MENU;
}


void Menu::draw(Renderer* renderer){

    renderer->drawBackground(menubg);
    draw_buttons(renderer);
    draw_battery_level(renderer);
    





    renderer->display();
}

GameState Menu::get_game_state(){
    return state;
}