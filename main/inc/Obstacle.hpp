#pragma once

#include <stdint.h>

struct Hitbox{
    int hitbox_offset_x;
    int hitbox_offset_y;
    uint8_t width;
    uint8_t height;
};

struct Obstacle{
    
    int pos_x;
    int pos_y;
    uint8_t width;
    uint8_t height;
    const uint16_t* sprite;
    bool is_active;
    Hitbox hitbox;
};



struct Decoration{
    
    int pos_x;
    int pos_y;
    uint8_t width;
    uint8_t height;
    const uint16_t* sprite;
    bool is_active;
};

