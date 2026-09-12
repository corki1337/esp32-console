#pragma once
#include "IGame.hpp"
#include "Button.hpp"
#include "Slider.hpp"



class Settings : public IGame{
public:

    void init() override;

    ChosenGame update(GameInput input, SoundPlayer* soundplayer) override;

    void draw(Renderer* renderer) override;    

    GameState get_game_state() override;

private:

    void init_buttons();
    void draw_buttons(Renderer *renderer);

    void init_sliders();
    void draw_sliders(Renderer *renderer);

    void update_battery();
    void draw_battery_level(Renderer *renderer);

    GameState state;

    uint8_t actbutton;
    static const uint8_t INPUT_LIMIT = 10;
    uint8_t input_limiter;

    static const uint8_t BUTTON_WIDTH = 100;
    static const uint8_t BUTTON_HEIGHT = 20;
    static const uint8_t BUTTON_COUNT = 1;

    static const uint8_t SLIDER_WIDTH = 100;
    static const uint8_t SLIDER_HEIGHT = 20;
    static const uint8_t SLIDER_COUNT = 2;

    uint16_t battery_voltage;
    uint16_t battery_tick;

    uint8_t is_muted;

    Button buttons[BUTTON_COUNT];
    Slider sliders[SLIDER_COUNT];
};
