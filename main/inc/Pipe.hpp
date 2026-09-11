#pragma once
#include <stdint.h>

struct Pipe{
    int xpos;
    int ygappos;
    uint16_t gapheight;
    bool is_passed;
};