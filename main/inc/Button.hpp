#pragma once
#include <stdint.h>

struct Button{
    int x;
    int y;
    int width;
    int height;
    const uint16_t *sprites[2];
};