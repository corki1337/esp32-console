#pragma once

#define MAGIC_COLOR 0x1ff8

extern "C"{
    #include "lcd.h"
}

class Renderer{
public:

    Renderer() {buffer = nullptr;}

    void init();

    void drawPixel(int x, int y, uint16_t color) {lcd_put_pixel(x, y, color);}

    void clearScreen(uint16_t color) {lcd_clear_screen(color);}

    void drawRect(int x, int y, int width, int height, uint16_t color);

    void drawSprite(int x, int y, int width, int height, const uint16_t* sprite);

    // draws sprite without checking magic color
    void drawSpriteWOMC(int x, int y, int width, int height, const uint16_t* sprite);

    void drawBackground(const uint16_t* sprite);

    void drawNumber(int x, int y, uint32_t number, uint16_t color);

    void display();

private:
    uint16_t* buffer;
};