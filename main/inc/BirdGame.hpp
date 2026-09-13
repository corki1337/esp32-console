#pragma once

#include <stdint.h>
#include "IGame.hpp"
#include "Renderer.hpp"
#include "Obstacle.hpp"
#include "Pipe.hpp"

#define SUBPIXEL_SHIFT 4


class BirdGame : public IGame{
public:

    BirdGame();

    void init() override;

    ChosenGame update(GameInput input, SoundPlayer* soundplayer) override;

    void draw(Renderer* renderer) override;

    GameState get_game_state() override;



private:

    void init_pipes();
    void spawn_pipe(int idx, int xpos, int ygappos, uint16_t gapheight);
    void draw_pipe(Renderer* renderer, int idx);
    void draw_pipes(Renderer* renderer);
    void update_pipes();
    int16_t get_prev_pipe_gap_pos(int idx);
    int16_t get_prev_pipe_xpos(int idx);


    void init_clouds();
    void draw_clouds(Renderer* renderer);
    int get_prev_cloud_posx(int idx);
    void update_clouds();

    void init_bird();
    void draw_bird(Renderer* renderer);
    void control_bird(GameInput input, SoundPlayer* soundplayer);
    void update_bird(GameInput input, SoundPlayer* soundplayer);
    void update_score();

    void check_collision(SoundPlayer *soundplayer);



    // game variables
    GameState state;
    uint16_t highscore;
    uint16_t score;
    uint8_t speed;
    int gravity;
    uint8_t restart_tick;
    uint16_t lose_tick;
    uint16_t start_tick;


    // pipes variables
    static const uint8_t MAX_PIPES = 3;
    Pipe pipes[MAX_PIPES];
    uint8_t pipe_spacing;
    uint8_t gap_height;

    // clouds
    static const uint8_t MAX_CLOUDS = 6;
    static const uint8_t MIN_Y_CLOUD = 5;
    static const uint8_t MAX_Y_CLOUD = 110;
    Decoration clouds[MAX_CLOUDS];
    uint8_t clouds_spacing;


    // bird variales
    static const int BIRD_WIDTH = 17;
    static const int BIRD_HEIGHT = 13;
    static const int BIRD_HITBOX_XOFFSET = 2;
    static const int BIRD_HITBOX_YOFFSET = 2;
    static const int BIRD_HITBOX_WIDTH = 9;
    static const int BIRD_HITBOX_HEIGHT = 9;
    static const int BIRD_XPOS = 30;
    static const int BIRD_CONTROL_LIMIT = 10;
    static const int GRAVITY = 1;
    uint16_t bird_animation_tick; 
    uint8_t bird_curr_animation;
    uint8_t bird_last_control_tick;
    int bird_posy;
    int bird_vel;





    


};