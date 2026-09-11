#include "Renderer.hpp"
#include <cstring>

#define NUM_HEIGHT 7
static const uint8_t num_widths[] = {5,3,4,4,5,4,4,4,4,4};


static const uint8_t zero[5 * NUM_HEIGHT] ={
    0,1,1,1,0,
    1,0,0,0,1,
    1,0,0,0,1,
    1,0,0,0,1,
    1,0,0,0,1,
    1,0,0,0,1,
    0,1,1,1,0
};
static const uint8_t one[3 * NUM_HEIGHT] = {
    1,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    0,1,0,
    1,1,1,
};
static const uint8_t two[4 * NUM_HEIGHT] = {
    0,1,1,0,
    1,0,0,1,
    0,0,0,1,
    0,0,1,0,
    0,1,0,0,
    1,0,0,0,
    1,1,1,1
};
static const uint8_t three[4 * NUM_HEIGHT] = {
    0,1,1,1,
    1,0,0,1,
    0,0,1,0,
    0,0,1,1,
    0,0,0,1,
    0,0,0,1,
    1,1,1,0
};
static const uint8_t four[5 * NUM_HEIGHT] = {
    0,0,0,1,0,
    0,0,1,1,0,
    0,1,0,1,0,
    1,0,0,1,0,
    1,1,1,1,1,
    0,0,0,1,0,
    0,0,0,1,0
};
static const uint8_t five[4 * NUM_HEIGHT] = {
    0,1,1,1,
    1,0,0,0,
    1,1,1,0,
    0,0,1,1,
    0,0,0,1,
    0,0,0,1,
    1,1,1,0
};
static const uint8_t six[4 * NUM_HEIGHT] = {
    0,0,1,0,
    0,1,0,0,
    1,0,1,0,
    1,1,0,1,
    1,0,0,1,
    1,0,0,1,
    0,1,1,0
};
static const uint8_t seven[4 * NUM_HEIGHT] = {
    1,1,1,1,
    0,0,0,1,
    0,0,0,1,
    0,0,1,0,
    0,0,1,0,
    0,0,1,0,
    1,0,0,0
};
static const uint8_t eight[4 * NUM_HEIGHT] = {
    0,1,1,0,
    1,0,0,1,
    1,0,0,1,
    0,1,1,0,
    1,0,0,1,
    1,0,0,1,
    0,1,1,0
};
static const uint8_t nine[4 * NUM_HEIGHT] = {
    0,1,1,0,
    1,0,0,1,
    1,0,0,1,
    1,0,0,1,
    0,1,1,1,
    0,0,1,0,
    0,1,0,0
};


static const uint8_t* digits[] = {zero, one, two, three, four, five, six, seven, eight, nine};

void Renderer::init(){
    lcd_init();
    buffer = lcd_get_draw_buffer();
}

void Renderer::display(){
    lcd_copy();
    buffer = lcd_get_draw_buffer();
}

void Renderer::drawRect(int x, int y, int width, int height, uint16_t color){

    for(int i = x; i < x + width; i++){
        if(i < 0 || i >= LCD_WIDTH) continue;
        for(int j = y; j < y + height; j++){
            if(j < 0 || j >= LCD_HEIGHT) continue;
            buffer[i + j * LCD_WIDTH] = color;
        }
    }
}

void Renderer::drawSprite(int x, int y, int width, int height, const uint16_t* sprite){

    int i_start = x < 0 ? -x : 0;
    int i_end = (x + width > LCD_WIDTH) ? LCD_WIDTH - x :  width;

    int j_start = y<0 ? -y : 0;
    int j_end = (y + height > LCD_HEIGHT) ? LCD_HEIGHT - y : height;

    if(i_start >= i_end || j_start >= j_end) return;

    for(int j = j_start; j < j_end; j++){
        for(int i = i_start; i < i_end; i++){ 
            if(sprite[i + width * j] != MAGIC_COLOR){
                buffer[x + i + (y + j)*LCD_WIDTH] = sprite[i + width * j];
            }
        }
    }
}

void Renderer::drawSpriteWOMC(int x, int y, int width, int height, const uint16_t* sprite){

    int i_start = x < 0 ? -x : 0;
    int i_end = (x + width > LCD_WIDTH) ? LCD_WIDTH - x :  width;

    int j_start = y<0 ? -y : 0;
    int j_end = (y + height > LCD_HEIGHT) ? LCD_HEIGHT - y : height;

    if(i_start >= i_end || j_start >= j_end) return;

    for(int j = j_start; j < j_end; j++){
        for(int i = i_start; i < i_end; i++){ 
            buffer[x + i + (y + j)*LCD_WIDTH] = sprite[i + width * j];
        }
    }
}



void Renderer::drawBackground(const uint16_t* background){
    memcpy(buffer, background, LCD_WIDTH * LCD_HEIGHT * sizeof(uint16_t));
}

void Renderer::drawNumber(int x, int y, uint32_t number, uint16_t color){

    
    uint8_t digit_count = 0;
    uint8_t digs[10] = {};

    if(number == 0){
        digs[0] = 0;
        digit_count = 1;
    }else{
        while(number > 0){
            digs[digit_count] = number % 10;
            number /= 10;
            digit_count++;
        }
    }   
    int actx = x;    

    int j_start = (y < 0) ? - y : 0;
    int j_end = (y + NUM_HEIGHT > LCD_HEIGHT) ? LCD_HEIGHT - y : NUM_HEIGHT;

    for(int idx = digit_count-1;idx >= 0;idx--){

        uint8_t digit = digs[idx];
        uint8_t width = num_widths[digit];

        int i_start = (actx < 0) ? -actx : 0;
        int i_end = (actx + width > LCD_WIDTH) ? LCD_WIDTH - actx : width;

        for(int j = j_start; j < j_end; j++){
            for(int i = i_start; i < i_end; i++){
                if(digits[digit][i + width * j]){
                    buffer[actx + i + (j + y) * LCD_WIDTH] = color;
                }
            }
        } 
        actx+=width + 1;
    }
}