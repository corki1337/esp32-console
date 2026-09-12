#pragma once
#include <stdint.h>

struct Slider{
    int x;
    int y;
    int width;
    int height;
    int progress;

    const uint16_t *slidersprite;

    int textwidth;
    int textheight;
    const uint16_t *textsprite;
};