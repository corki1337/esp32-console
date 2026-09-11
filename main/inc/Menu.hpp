#pragma once

#include "IGame.hpp"
#include "Button.hpp"

class Menu : public IGame{

public:

    void init() override;

    ChosenGame update(GameInput input, SoundPlayer* soundplayer) override;

    void draw(Renderer* renderer) override;

    GameState get_game_state() override;

private:

    void init_buttons();
    void draw_battery_level(Renderer *renderer);
    void draw_buttons(Renderer *renderer);



    GameState state;

    uint8_t actbutton;
    static const uint8_t INPUT_LIMIT = 10;
    uint8_t input_limiter;

    static const uint8_t BUTTON_WIDTH = 100;
    static const uint8_t BUTTON_HEIGHT = 20;
    static const uint8_t BUTTON_COUNT = 4;

    uint16_t battery_voltage;
    uint16_t battery_tick;

    Button buttons[BUTTON_COUNT];



};