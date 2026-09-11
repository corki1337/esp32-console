#pragma once

#include "Renderer.hpp"
#include "SoundPlayer.hpp"

typedef enum{
    START,
    GAME_OVER,
    PLAYING
} GameState;

enum class ChosenGame{
    MENU,
    BIRD_GAME,
    DINO_GAME,
    SNAKE_GAME,
    SETTINGS
};

struct GameInput{
    bool up;
    bool right;
    bool down;
    bool left;
    bool action;
    bool back;
};

class IGame{
    
public:
    virtual ~IGame() = default;

    virtual void init() = 0;

    virtual ChosenGame update(GameInput input, SoundPlayer* soundplayer) = 0;

    virtual void draw(Renderer* renderer) = 0;    

    virtual GameState get_game_state() = 0;
};