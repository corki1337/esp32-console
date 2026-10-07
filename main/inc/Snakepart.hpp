#pragma once
#include <stdint.h>

enum class Bodypart : uint8_t{
    HEAD,
    BODY,
    TAIL
};

enum class Orientation : uint8_t{
    UP,
    RIGHT,
    DOWN,
    LEFT
};


struct Snakepart{
    uint8_t spriteid;
    bool is_active;
    int8_t xgrid;
    int8_t ygrid;
    Bodypart bodypart;
    Orientation orientation;

};