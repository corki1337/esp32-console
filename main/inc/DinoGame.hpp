#pragma once

#include <stdint.h>
#include "IGame.hpp"
#include "Renderer.hpp"
#include "Obstacle.hpp"


#define MIN_SAND 107
#define MAX_SAND 122

#define MIN_SKY 15
#define MAX_SKY 35

#define GROUND 103


class DinoGame : public IGame{

public:

    DinoGame();

    void init() override;

    ChosenGame update(GameInput input, SoundPlayer *soundplayer) override;

    void draw(Renderer* renderer) override;

    GameState get_game_state() override;

private:

    void check_collision(SoundPlayer *soundplayer);

    void update_grains();

    void update_clouds();

    void update_dino(GameInput input, SoundPlayer* soundplayer);

    void draw_decoration(Renderer* renderer, Decoration decoration);

    void draw_obstacle(Renderer* renderer, Obstacle obstacle);

    void init_random_grains();

    void init_random_clouds();

    void init_cacti();

    void dino_control(GameInput input, SoundPlayer* soundplayer);

    void draw_dino(Renderer* renderer);

    void update_cacti();

    void render_decorations(Renderer* renderer);


    Renderer renderer_object;

    int dino_pos_x;
    int dino_pos_y;
    int dino_width;
    int dino_height;
    Hitbox dino_hitbox;
    int dino_vel_y;
    bool dino_is_on_ground;
    int gravity;
    uint8_t dino_act_sprite;
    int dino_sprite_timer;


    GameState state;
    uint32_t score;
    uint16_t highscore;


    uint8_t speed;

    uint16_t lose_tick;
    uint16_t start_tick;
    uint8_t game_started_tick;

    static const int MAX_CACTUS = 4;
    Obstacle cacti[MAX_CACTUS];
    static const int MAX_CLOUDS = 3;
    Decoration clouds[MAX_CLOUDS];
    static const int MAX_GRAINS = 7;
    Decoration grains[MAX_GRAINS];
    Hitbox cactus_hitbox;
    


};