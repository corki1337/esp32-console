
#include "SnakeGame.hpp"
#include "sprites.hpp"
#include "sounds.hpp"
#include "esp_random.h"


static const uint16_t *headsprites[4] = {pickleheadup, pickleheadright, pickleheaddown, pickleheadleft};
static const uint16_t *body1sprites[4] = {picklebody1up, picklebody1right, picklebody1down, picklebody1left};
static const uint16_t *body2sprites[4] = {picklebody2up, picklebody2right, picklebody2down, picklebody2left};
static const uint16_t *body3sprites[4] = {picklebody3up, picklebody3right, picklebody3down, picklebody3left};
static const uint16_t *tailsprites[4] = {pickletailup, pickletailright, pickletaildown, pickletailleft};

#define AVAILABLEBODYSPRITES 3


static int getRandomRange(int min, int max) {  
    return min + (esp_random() % (max - min + 1));
}


const uint16_t* SnakeGame::get_snake_corner(int dx1, int dy1, int dx2, int dy2){
    if(dx1 + dx2 == 1){
        if(dy1 + dy2 == 1){
            return pickledownright;
        }else{
            return pickleupright;
        }
    }else{
        if(dy1 + dy2 == 1){
            return pickledownleft;
        }else{
            return pickleupleft;
        }
    }
}

const uint16_t* const* SnakeGame::get_snake_sprites(uint8_t spriteid){
    switch(spriteid){
        case 0:
            return headsprites;
        case 1: 
            return body1sprites;
        case 2:
            return body2sprites;
        case 3:
            return body3sprites;
        case 4:
            return tailsprites;
    }
    return tailsprites;
}




SnakeGame::SnakeGame(){
    
}

void SnakeGame::spawn_new_apple(){
    if(snakelen < GRIDHEIGHT * GRIDWIDTH - 1){
        while(true){
            int16_t newapple = getRandomRange(0, GRIDHEIGHT * GRIDWIDTH - 1);
            if(gamegrid[newapple] == 0){
                appley = newapple / GRIDWIDTH;
                applex = newapple % GRIDWIDTH;
                gamegrid[newapple] = 2;
                break;
            }
        }
    }
}


void SnakeGame::init_snake(){

    snakelen = 3;
    for(int i = 0; i < MAX_SNAKE; i++){
        snake[i].is_active = false;
    }

    snake[0] = {
        .spriteid = 0,
        .is_active = true,
        .xgrid = 10,
        .ygrid = 7,
        .bodypart = Bodypart::HEAD,
        .orientation = Orientation::UP
    };
    snake[snakelen-1] = {
        .spriteid = 4,
        .is_active = true,
        .xgrid = 10,
        .ygrid = 9,
        .bodypart = Bodypart::TAIL,
        .orientation = Orientation::UP
    };
    snake[1] = {
        .spriteid = 1,
        .is_active = true,
        .xgrid = 10,
        .ygrid = 8,
        .bodypart = Bodypart::BODY,
        .orientation = Orientation::UP
    };


    applex = getRandomRange(8, GRIDWIDTH - 2);
    appley = getRandomRange(1, GRIDHEIGHT - 2);
    
    gamegrid[10 + GRIDWIDTH * 7] = 1;
    gamegrid[10 + GRIDWIDTH * 8] = 1;
    gamegrid[10 + GRIDWIDTH * 9] = 1;

    
    spawn_new_apple();


}

uint8_t SnakeGame::get_snake_sprite_orientation(uint16_t snakeidx){
    if(snake[snakeidx].orientation == Orientation::UP){
        return 0;
    }else if(snake[snakeidx].orientation == Orientation::RIGHT){
        return 1;
    }else if(snake[snakeidx].orientation == Orientation::DOWN){
        return 2;
    }else{
        return 3;
    }
}

void SnakeGame::draw_snake(Renderer *renderer){


    for(int i = 0; i < snakelen; i++){
        if(!snake[i].is_active){
            break;
        }

        if(i == 0){
            renderer->drawSprite(XGRIDSTART + snake[i].xgrid * SQUARESIZE, YGRIDSTART + snake[i].ygrid * SQUARESIZE, SQUARESIZE, SQUARESIZE, get_snake_sprites(snake[i].spriteid)[get_snake_sprite_orientation(i)]);
        }else if(i == snakelen - 1){
            int dx = snake[i - 1].xgrid - snake[i].xgrid;
            int dy = snake[i - 1].ygrid - snake[i].ygrid;

            uint8_t tail_orientation = 0;
            if (dx == 1)  tail_orientation = 1;      
            else if (dy == 1)  tail_orientation = 2;
            else if (dx == -1) tail_orientation = 3; 

            renderer->drawSprite(XGRIDSTART + snake[i].xgrid * SQUARESIZE, YGRIDSTART + snake[i].ygrid * SQUARESIZE, SQUARESIZE, SQUARESIZE, get_snake_sprites(snake[i].spriteid)[tail_orientation]);
        }else{
            int dx1 = snake[i - 1].xgrid - snake[i].xgrid;
            int dy1 = snake[i - 1].ygrid - snake[i].ygrid;
            int dx2 = snake[i + 1].xgrid - snake[i].xgrid;
            int dy2 = snake[i + 1].ygrid - snake[i].ygrid;

            if(dx1 == -dx2 && dy1 == -dy2){
                renderer->drawSprite(XGRIDSTART + snake[i].xgrid * SQUARESIZE, YGRIDSTART + snake[i].ygrid * SQUARESIZE, SQUARESIZE, SQUARESIZE, get_snake_sprites(snake[i].spriteid)[get_snake_sprite_orientation(i)]);
            }else{
                renderer->drawSprite(XGRIDSTART + snake[i].xgrid * SQUARESIZE, YGRIDSTART + snake[i].ygrid * SQUARESIZE, SQUARESIZE, SQUARESIZE, get_snake_corner(dx1, dy1, dx2, dy2));
            }
        }

        
    }
}

void SnakeGame::draw_apple(Renderer *renderer){
    renderer->drawSprite(XGRIDSTART + applex * SQUARESIZE, YGRIDSTART + appley * SQUARESIZE, SQUARESIZE, SQUARESIZE, japko);
}


void SnakeGame::add_segment(){
    snake[snakelen - 1].bodypart = Bodypart::BODY;
    snake[snakelen - 1].spriteid = getRandomRange(1, AVAILABLEBODYSPRITES);
    

    snake[snakelen].is_active = true;
    snake[snakelen].bodypart = Bodypart::TAIL;
    snake[snakelen].orientation = prev_tail_orientation;
    snake[snakelen].spriteid = 4;
    snake[snakelen].xgrid = prev_tail_x;
    snake[snakelen].ygrid = prev_tail_y;

    gamegrid[prev_tail_x + GRIDWIDTH * prev_tail_y] = 1;
    snakelen++;
    score++;


}

void SnakeGame::check_collision(){
    if(gamegrid[snake[0].xgrid + snake[0].ygrid * GRIDWIDTH] == 1){
        state = GAME_OVER;
    }else if(gamegrid[snake[0].xgrid + snake[0].ygrid * GRIDWIDTH] == 2){
        add_segment();

        spawn_new_apple();
        
    }
}



void SnakeGame::init(){
    init_snake();
    score = 0;
    gametick = 0;
}

void SnakeGame::update_snake(GameInput input, SoundPlayer *soundplayer){

    bool just_pressed_up    = input.up    && !prev_input.up;
    bool just_pressed_down  = input.down  && !prev_input.down;
    bool just_pressed_left  = input.left  && !prev_input.left;
    bool just_pressed_right = input.right && !prev_input.right;

    prev_input = input;

    if(moves_count < 2){
        Orientation last_direction = (moves_count > 0) ? move_queue[moves_count - 1] : snake[0].orientation;

        if(last_direction == Orientation::UP || last_direction == Orientation::DOWN){
            if(just_pressed_left){
                move_queue[moves_count++] = Orientation::LEFT;
            }else if(just_pressed_right){
                move_queue[moves_count++] = Orientation::RIGHT;
            }
        }else{
            if(just_pressed_up){
                move_queue[moves_count++] = Orientation::UP;
            }else if(just_pressed_down){
                move_queue[moves_count++] = Orientation::DOWN;
            }
        }

    }

    if(gametick > 7){
        prev_tail_x = snake[snakelen - 1].xgrid;
        prev_tail_y = snake[snakelen - 1].ygrid;
        prev_tail_orientation = snake[snakelen - 1].orientation;

        gamegrid[prev_tail_x + GRIDWIDTH * prev_tail_y] = 0;



        for(int i = snakelen - 1; i > 0; i--){
            snake[i].xgrid = snake[i-1].xgrid;
            snake[i].ygrid = snake[i-1].ygrid;
            snake[i].orientation = snake[i-1].orientation;

            


        }
        
        if(moves_count > 0){
            snake[0].orientation = move_queue[0];

            if(moves_count == 2){
                move_queue[0] = move_queue[1];
            }
            moves_count--;
        }

        switch(snake[0].orientation){
            case Orientation::UP:
                snake[0].ygrid--;
                break;
            case Orientation::DOWN:
                snake[0].ygrid++;
                break;
            case Orientation::RIGHT:
                snake[0].xgrid++;
                break;
            case Orientation::LEFT:
                snake[0].xgrid--;
                break;
        }
        check_collision();
        gamegrid[snake[0].xgrid + GRIDWIDTH * snake[0].ygrid] = 1;

        gametick=0;
    }else{
        gametick++;
    }
}

ChosenGame SnakeGame::update(GameInput input, SoundPlayer *soundplayer){



    update_snake(input, soundplayer);
    




    return ChosenGame::SNAKE_GAME;
}

GameState SnakeGame::get_game_state(){
    return GameState::PLAYING;
}

void SnakeGame::draw(Renderer *renderer){
    
    renderer->drawBackground(picklebg);

    draw_snake(renderer);
    draw_apple(renderer);

    renderer->display();

}