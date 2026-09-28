#pragma once

#include <stdint.h>
#include "IGame.hpp"
#include "Renderer.hpp"
#include "SoundPlayer.hpp"
#include "Snakepart.hpp"



class SnakeGame : public IGame{

public:

    SnakeGame();

    void init() override;

    ChosenGame update(GameInput input, SoundPlayer *soundplayer) override;

    void draw(Renderer *renderer) override;

    GameState get_game_state() override;


private:

    const uint16_t* const* get_snake_sprites(uint8_t spriteid);

    const uint16_t* get_snake_corner(int dx1, int dy1, int dx2, int dy2);

    uint8_t get_snake_sprite_orientation(uint16_t snakeidx);

    void init_snake();

    void update_snake(GameInput input, SoundPlayer *soundplayer);

    void draw_snake(Renderer *renderer);

    void add_segment();

    void check_collision();

    void draw_apple(Renderer *renderer);

    void spawn_new_apple();

    static const uint16_t XGRIDSTART = 7;
    static const uint16_t YGRIDSTART = 15;
    static const uint16_t SQUARESIZE = 7;

    static const uint16_t GRIDWIDTH = 21;
    static const uint16_t GRIDHEIGHT = 15;



    int8_t applex;
    int8_t appley;


    static const uint16_t MAX_SNAKE = 315;
    Snakepart snake[MAX_SNAKE];

    uint8_t gamegrid[MAX_SNAKE] = {0};

    GameInput prev_input = {};
    Orientation move_queue[2];
    int moves_count = 0;

    GameState state;

    uint8_t prev_tail_x;
    uint8_t prev_tail_y;
    Orientation prev_tail_orientation;

    uint16_t score;
    uint16_t snakelen;

    uint16_t gametick;

};