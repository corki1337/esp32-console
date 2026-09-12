#include "BirdGame.hpp"
#include "nvsmem.h"
#include "sprites.hpp"
#include "esp_random.h"
#include "sounds.hpp"
#define PIPE_WIDTH 30

static const uint16_t* available_clouds[] = {cloud1, cloud2};
static const uint8_t CLOUDS_SPRITES = (uint8_t)sizeof(available_clouds)/sizeof(uint16_t*) - 1;

static const uint16_t* bird0[]= {bird00,bird01};
static const uint16_t* bird1[]= {bird10,bird11};
static const uint16_t* bird2[]= {bird20,bird21};

static int getRandomRange(int min, int max) {  
    return min + (esp_random() % (max - min + 1));
}

BirdGame::BirdGame(){
    highscore = nvs_read("BIRD", "HIGHSCORE");
    if(highscore == UINT16_MAX){
        highscore = 0;
    }
}

int16_t BirdGame::get_prev_pipe_gap_pos(int idx){
    return (idx==0) ? pipes[MAX_PIPES-1].ygappos : pipes[idx-1].ygappos;

}

int16_t BirdGame::get_prev_pipe_xpos(int idx){
    return (idx==0) ? pipes[MAX_PIPES-1].xpos : pipes[idx-1].xpos;

}

void BirdGame::spawn_pipe(int idx, int xpos, int ygappos, uint16_t gapheight){
    pipes[idx].xpos = xpos;
    pipes[idx].ygappos = ygappos;
    pipes[idx].gapheight = gapheight;
}

void BirdGame::draw_pipe(Renderer* renderer, int idx){
    int i = 0;
    int posx = pipes[idx].xpos;
    if(pipes[idx].ygappos >= 11){
        for(;i < pipes[idx].ygappos - 10; i++){
            renderer->drawSpriteWOMC(posx+1,i,PIPE_WIDTH-2,1, pipe28x1);
        }
    }
    renderer->drawSpriteWOMC(posx, i, PIPE_WIDTH, 10, pipe30x10);
    i += 10 + pipes[idx].gapheight;
    renderer->drawSpriteWOMC(posx, i, PIPE_WIDTH, 10, pipe30x10);
    i += 10;
    if(i < LCD_HEIGHT){
        for(;i < LCD_HEIGHT;i++){
            renderer->drawSpriteWOMC(posx+1, i, PIPE_WIDTH-2, 1, pipe28x1);
        }
    }

    //renderer->drawRect(pipes[idx].xpos, pipes[idx].ygappos, 30,pipes[idx].gapheight, RED);

}

void BirdGame::draw_pipes(Renderer* renderer){
    for(int i = 0; i < MAX_PIPES; i++){
        draw_pipe(renderer, i);
    }
}

void BirdGame::init_pipes(){
    int ygappos = getRandomRange(40, 70);
    int xpos = 160;
    
    for(int i = 0; i < MAX_PIPES; i++){
        
        pipes[i].xpos = xpos;
        pipes[i].ygappos = ygappos;
        pipes[i].gapheight = gap_height;
        pipes[i].is_passed = false;
        ygappos += getRandomRange(0, 50) - 25;
        ygappos = (ygappos < 11) ? 11 : ygappos;
        ygappos = (ygappos + gap_height + 11 >= LCD_HEIGHT) ? LCD_HEIGHT - 11 - gap_height : ygappos;
        xpos += pipe_spacing;

    }
}

void BirdGame::update_pipes(){

    
    if(score < 10){
        gap_height = 45;
    }else if(score < 20){
        gap_height = 40;
    }else if(score < 40){
        gap_height = 35;
    }else if(score < 50){
        gap_height = 30;
    }else{
        gap_height = 25;
    }
    int ygappos;
    for(int i = 0; i < MAX_PIPES; i++){
        pipes[i].xpos -= speed;
        if(pipes[i].xpos + PIPE_WIDTH < 0){
            pipes[i].xpos = get_prev_pipe_xpos(i) + pipe_spacing;
            ygappos = get_prev_pipe_gap_pos(i) + getRandomRange(0,50) - 25;
            ygappos = (ygappos < 11) ? 11 : ygappos;
            ygappos = (ygappos + gap_height + 11 >= LCD_HEIGHT) ? LCD_HEIGHT - 11 - gap_height : ygappos;
            pipes[i].ygappos = ygappos;
            pipes[i].is_passed = false;
            pipes[i].gapheight = gap_height;
        }
    }
}

void BirdGame::init_clouds(){
    clouds_spacing = 30;
    int xposition = 10;
    for(int i = 0; i < MAX_CLOUDS; i++){
        clouds[i].pos_x = xposition<<SUBPIXEL_SHIFT;
        clouds[i].pos_y = getRandomRange(MIN_Y_CLOUD, MAX_Y_CLOUD);
        clouds[i].width = 28;
        clouds[i].height = 13;
        clouds[i].sprite = available_clouds[getRandomRange(0, CLOUDS_SPRITES)];
        clouds[1].is_active = true;
        xposition += clouds_spacing + getRandomRange(0, 20);
    }
}

void BirdGame::draw_clouds(Renderer* renderer){
    for(int i = 0; i < MAX_CLOUDS; i++){
        renderer->drawSprite(clouds[i].pos_x>>SUBPIXEL_SHIFT, clouds[i].pos_y, clouds[i].width, clouds[i].height, clouds[i].sprite);
    }
}

int BirdGame::get_prev_cloud_posx(int idx){
    return (idx == 0) ? clouds[MAX_CLOUDS-1].pos_x>>SUBPIXEL_SHIFT : clouds[idx-1].pos_x>>SUBPIXEL_SHIFT;
}

void BirdGame::update_clouds(){
    for(int i = 0; i < MAX_CLOUDS; i++){
        clouds[i].pos_x -= 2;
        if((clouds[i].pos_x>>SUBPIXEL_SHIFT) + clouds[i].width < 0){
            clouds[i].pos_x = (clouds_spacing + get_prev_cloud_posx(i) + getRandomRange(0, 20))<<SUBPIXEL_SHIFT;
            clouds[i].pos_y = getRandomRange(MIN_Y_CLOUD, MAX_Y_CLOUD);
            clouds[i].sprite = available_clouds[getRandomRange(0, CLOUDS_SPRITES)];

        }
    }
}

void BirdGame::init_bird(){
    bird_animation_tick = 0;
    bird_posy = 60<<SUBPIXEL_SHIFT;
    bird_vel = 1;
    bird_last_control_tick = 0;
}

void BirdGame::draw_bird(Renderer* renderer){
    if (state == PLAYING || state == START){
        bird_animation_tick++;
    }

    if(bird_animation_tick  < 10){



        bird_curr_animation = 0;
    }else if(bird_animation_tick <= 20){
        bird_curr_animation = 1;
    }else{
        bird_animation_tick = 0;
    }

    if(bird_vel >= 2){
        renderer->drawSprite(BIRD_XPOS, bird_posy>>SUBPIXEL_SHIFT, BIRD_WIDTH, BIRD_HEIGHT, bird2[bird_curr_animation]);
    }else if(bird_vel <= -2){
        renderer->drawSprite(BIRD_XPOS, bird_posy>>SUBPIXEL_SHIFT, BIRD_WIDTH, BIRD_HEIGHT, bird0[bird_curr_animation]);
    }else{
        renderer->drawSprite(BIRD_XPOS, bird_posy>>SUBPIXEL_SHIFT, BIRD_WIDTH, BIRD_HEIGHT, bird1[bird_curr_animation]);
    }


}

void BirdGame::control_bird(GameInput input, SoundPlayer* soundplayer){

    if(bird_vel <= 0 && (input.action || input.up)){
        bird_vel = 15;
        bird_last_control_tick = 0;
        soundplayer->playSoundVolume((int16_t*)jump, sizeof(jump));
    }else if(input.action || input.up){
        bird_vel += 15;
        bird_last_control_tick = 0;
        soundplayer->playSoundVolume((int16_t*)jump, sizeof(jump));
    }
    
    
}

void BirdGame::update_bird(GameInput input, SoundPlayer* soundplayer){

    if(bird_last_control_tick > BIRD_CONTROL_LIMIT){
        control_bird(input, soundplayer);
        
    }

    bird_last_control_tick++;


    bird_vel -= GRAVITY;
    bird_posy -= bird_vel;
}

void BirdGame::check_collision(SoundPlayer *soundplayer){

    int birdyposition = bird_posy>>SUBPIXEL_SHIFT;

    for(int i = 0; i < MAX_PIPES; i++){
        if(!pipes[i].is_passed){
            if((BIRD_XPOS + BIRD_HITBOX_XOFFSET + BIRD_HITBOX_WIDTH >= pipes[i].xpos) && (BIRD_XPOS + BIRD_HITBOX_XOFFSET <= pipes[i].xpos + PIPE_WIDTH)){
                if((birdyposition + BIRD_HITBOX_YOFFSET <= pipes[i].ygappos) || (birdyposition + BIRD_HITBOX_YOFFSET + BIRD_HITBOX_HEIGHT >= pipes[i].ygappos + pipes[i].gapheight)){
                    soundplayer->playSoundVolume((int16_t*)hitHurt_2_, sizeof(hitHurt_2_));
                    state = GAME_OVER;
                    restart_tick = 0;
                    lose_tick=0;
                    if(score > highscore){
                        highscore = score;
                        nvs_save("BIRD", "HIGHSCORE", score);
                    }
                }
            }
        }
    }
}

void BirdGame::update_score(){
    for(int i = 0; i < MAX_PIPES; i++){
        if(!pipes[i].is_passed){
            if(BIRD_XPOS >= pipes[i].xpos + PIPE_WIDTH){
                score++;
                pipes[i].is_passed = true;
            }
        }
    }

}

void BirdGame::init(){
    speed = 1;
    score = 0;
    pipe_spacing = 90;
    gap_height = 50;

    init_bird();    
    init_pipes();
    init_clouds();
    state = START;
}

ChosenGame BirdGame::update(GameInput input, SoundPlayer* soundplayer){
    switch(state){
        case PLAYING:
            update_clouds();
            update_pipes();
            update_bird(input, soundplayer);

            check_collision(soundplayer);
            update_score();
            


            return ChosenGame::BIRD_GAME;
        case GAME_OVER:
            lose_tick++;
            if(restart_tick > 15){
                if(input.action || input.up){
                    init();
                    state = PLAYING;
                    return ChosenGame::BIRD_GAME;
                }else if(input.back){
                    return ChosenGame::MENU;
                }
            }else{
                restart_tick++;
            }
            if(restart_tick > 200) restart_tick = 16;
            if(lose_tick > 100) lose_tick = 0;
            return ChosenGame::BIRD_GAME;
            
        case START:
            start_tick++;
            if(input.action || input.up){
                
                state = PLAYING;
            }else if(input.back){
                return ChosenGame::MENU;
            }
            if(start_tick > 200) start_tick = 0;
            return ChosenGame::BIRD_GAME;
    }
    return ChosenGame::BIRD_GAME;
}

void BirdGame::draw(Renderer* renderer){

    renderer->drawBackground(birdgametlo1);

    if(state == PLAYING || state == START){
        draw_clouds(renderer);
    }
    draw_pipes(renderer);
    draw_bird(renderer);

    switch(state){
        case PLAYING:
            renderer->drawNumber(10, 5, (uint32_t)score, BLACK);
            break;
        case GAME_OVER:
            renderer->drawSprite(19, 20, 122, 13, gameover);
            renderer->drawSprite(40, 60, 34, 7, score_text);
            renderer->drawNumber(75, 60, score, BLACK);
            renderer->drawSprite(14, 70, 25, 7, high_text);
            renderer->drawSprite(40, 70, 34, 7, score_text);
            renderer->drawNumber(75, 70, highscore, BLACK);


            if(lose_tick % 50 < 25){
                renderer->drawSprite(20, 90, 119, 11, press_button);
                renderer->drawSprite(30, 105, 99, 11, torestart);      
            }
            break;
        case START:
            renderer->drawSprite(14, 30, 25, 7, high_text);
            renderer->drawSprite(40, 30, 34, 7, score_text);
            renderer->drawNumber(75, 30, highscore, BLACK);

            if(start_tick % 50 < 25){
                renderer->drawSprite(20, 90, 119, 11, press_button);
                renderer->drawSprite(40, 105, 79, 11, tostart);  
            }
            break;
    }



    //renderer->drawRect(BIRD_XPOS + BIRD_HITBOX_XOFFSET, (bird_posy>>SUBPIXEL_SHIFT) + BIRD_HITBOX_YOFFSET, BIRD_HITBOX_WIDTH, BIRD_HITBOX_HEIGHT, GREEN);
    renderer->display();
}

GameState BirdGame::get_game_state(){
    return state;
}

