#include "DinoGame.hpp"
#include "esp_random.h"
#include "sprites.hpp"
#include "nvsmem.h"
#include "sounds.hpp"

#define SUBPIXEL_SHIFT 4


const uint16_t grain1[] = {BLACK, BLACK, BLACK, MAGIC_COLOR};
const uint16_t grain2[] = {BLACK, MAGIC_COLOR, BLACK, MAGIC_COLOR};
const uint16_t grain3[] = {BLACK, MAGIC_COLOR, MAGIC_COLOR, MAGIC_COLOR};
const uint16_t grain4[] = {BLACK, BLACK, MAGIC_COLOR, BLACK};

const static uint8_t GRAINS = 4;
const uint16_t* available_grains[] = {grain1, grain2, grain3, grain4};
const static uint8_t CLOUDS = 2;
const uint16_t* available_clouds[] = {cloud1, cloud2};
const uint16_t* dino_sprites[] = {dino1, dino2, dino3};
const static uint8_t CACTI = 1;
const uint16_t* available_cacti[] = {cactus1, cactus2};




static int getRandomRange(int min, int max) {  
    return min + (esp_random() % (max - min + 1));
}

static int getRandomBool(){
  return esp_random() & 1;
}

DinoGame::DinoGame(){
  highscore = nvs_read("DINO","HIGHSCORE");
  if(highscore == UINT16_MAX){
    highscore = 0;
  }
}

void DinoGame::init_random_grains(){
  for(int i = 0; i < MAX_GRAINS; i++){
    grains[i] = {
      .pos_x = getRandomRange((LCD_WIDTH/MAX_GRAINS * i )+1, (LCD_WIDTH/MAX_GRAINS * i) + 1 + LCD_WIDTH/MAX_GRAINS)<<SUBPIXEL_SHIFT,
      .pos_y = getRandomRange(MIN_SAND, MAX_SAND)<<SUBPIXEL_SHIFT,
      .width = 2,
      .height = 2,
      .sprite = available_grains[getRandomRange(0,3)],
      .is_active = true
    };
  }
}

void DinoGame::init_random_clouds(){
  
  int spacing = LCD_WIDTH / MAX_CLOUDS;

  int max_random_space = spacing - 28;



  for(int i = 0; i < MAX_CLOUDS; i++){
    int base_x = i * spacing;


    clouds[i] = {
      .pos_x = (getRandomRange(base_x, base_x + max_random_space)+160)<<SUBPIXEL_SHIFT,
      .pos_y = getRandomRange(MIN_SKY, MAX_SKY)<<SUBPIXEL_SHIFT,
      .width = 28,
      .height = 13,
      .sprite = available_clouds[getRandomRange(0,1)],
      .is_active = true
    };
  }
}

void DinoGame::init_cacti(){
  cactus_hitbox = {
    .hitbox_offset_x = 1,
    .hitbox_offset_y = 1,
    .width = 7,
    .height = 17
  };

  cacti[0] = {
    .pos_x = (160+10)<<SUBPIXEL_SHIFT,
    .pos_y = (GROUND - 18)<<SUBPIXEL_SHIFT,
    .width = 9,
    .height = 18,
    .sprite = available_cacti[getRandomRange(0,CACTI)],
    .is_active = true,
    .hitbox = cactus_hitbox
  };
  cacti[1] = {
    .pos_x = 160<<SUBPIXEL_SHIFT,
    .pos_y = (GROUND - 18)<<SUBPIXEL_SHIFT,
    .width = 9,
    .height = 18,
    .sprite = available_cacti[getRandomRange(0,CACTI)],
    .is_active = false,
    .hitbox = cactus_hitbox
  };
  cacti[2] = {
    .pos_x = (getRandomRange(250,330)+10)<<SUBPIXEL_SHIFT,
    .pos_y = (GROUND - 18)<<SUBPIXEL_SHIFT,
    .width = 9,
    .height = 18,
    .sprite = available_cacti[getRandomRange(0, CACTI)],
    .is_active = true,
    .hitbox = cactus_hitbox
  };
  cacti[3] = {
    .pos_x = getRandomRange(220,300)<<SUBPIXEL_SHIFT,
    .pos_y = (GROUND - 18)<<SUBPIXEL_SHIFT,
    .width = 9,
    .height = 18,
    .sprite = available_cacti[getRandomRange(0,CACTI)],
    .is_active = false,
    .hitbox = cactus_hitbox
  };

}

void DinoGame::init(){
    dino_hitbox = {
      .hitbox_offset_x=1,
      .hitbox_offset_y=1,
      .width=10,
      .height=18
    };

    dino_width = 14;
    dino_height = 20;
    dino_pos_x = 30<<SUBPIXEL_SHIFT;
    dino_pos_y = (GROUND-dino_height)<<SUBPIXEL_SHIFT;   
    speed = 20;
    gravity = 2;
    dino_vel_y = 0;
    dino_act_sprite = 0;
    dino_is_on_ground=true;
    score = 0;
    lose_tick = 0;
    start_tick = 0;
    

    init_random_grains();
    init_random_clouds();
    init_cacti();

    game_started_tick = 0;
    state = START;

}




void DinoGame::draw_decoration(Renderer* renderer, Decoration decoration){
  renderer->drawSprite(decoration.pos_x>>SUBPIXEL_SHIFT, decoration.pos_y>>SUBPIXEL_SHIFT, decoration.width, decoration.height, decoration.sprite);
}
void DinoGame::draw_obstacle(Renderer* renderer, Obstacle obstacle){
  renderer->drawSprite(obstacle.pos_x>>SUBPIXEL_SHIFT, obstacle.pos_y>>SUBPIXEL_SHIFT, obstacle.width, obstacle.height, obstacle.sprite);
}


void DinoGame::update_grains(){
  for(int i = 0; i < MAX_GRAINS; i++){
    if(grains[i].is_active){
      grains[i].pos_x -= speed;
      if(grains[i].pos_x < 0){
        grains[i].is_active = false;
      }
    }else{
      grains[i].pos_x = 159<<SUBPIXEL_SHIFT;
      grains[i].pos_y = getRandomRange(MIN_SAND, MAX_SAND)<<SUBPIXEL_SHIFT;
      grains[i].is_active = true;
      grains[i].sprite = available_grains[getRandomRange(0, 3)];
    }
  }
}


void DinoGame::update_clouds(){
  for(int i = 0; i < MAX_CLOUDS; i++){
    if(clouds[i].is_active){
      clouds[i].pos_x -= speed>>2;
      if((int)clouds[i].pos_x + (clouds[i].width<<SUBPIXEL_SHIFT) < 0){
        clouds[i].is_active = false;
      }
    }else{
      clouds[i].pos_x = 160<<SUBPIXEL_SHIFT;
      clouds[i].pos_y = getRandomRange(MIN_SKY, MAX_SKY)<<SUBPIXEL_SHIFT;
      clouds[i].is_active = true;
      clouds[i].sprite = available_clouds[getRandomRange(0, 1)];
    }
  }
}


void DinoGame::render_decorations(Renderer* renderer){
  for(int i = 0; i < MAX_GRAINS; i++){
    if(grains[i].is_active){
      draw_decoration(renderer, grains[i]);
    }
  }

  if(state == GAME_OVER) return;
  for(int i = 0; i < MAX_CLOUDS; i++){
    if(clouds[i].is_active){
      draw_decoration(renderer, clouds[i]);
    } 
  } 
  
  
}

void DinoGame::draw_dino(Renderer* renderer){
  renderer->drawSprite(dino_pos_x>>SUBPIXEL_SHIFT, dino_pos_y>>SUBPIXEL_SHIFT, dino_width, dino_height, dino_sprites[dino_act_sprite]);

}


GameState DinoGame::get_game_state(){
  return state;
}

void DinoGame::draw(Renderer* renderer){

    renderer->drawBackground(tlo1);


    for(int i = 0; i < MAX_CACTUS; i++){
      if(cacti[i].is_active){
        draw_obstacle(renderer, cacti[i]);
      }
    }


    render_decorations(renderer);

    draw_dino(renderer);

    

    switch(state){

    
    case GAME_OVER: 
      renderer->drawSprite(19, 10, 122, 13, gameover);
      renderer->drawSprite(40, 30, 34, 7, score_text);
      renderer->drawNumber(75, 30, score>>2, BLACK);
      renderer->drawSprite(14, 40, 25, 7, high_text);
      renderer->drawSprite(40, 40, 34, 7, score_text);
      renderer->drawNumber(75, 40, highscore, BLACK);


      if(lose_tick % 50 < 25){
        renderer->drawSprite(20, 60, 119, 11, press_button);
        renderer->drawSprite(30, 75, 99, 11, torestart);      
      }
      break;

    case PLAYING:
      renderer->drawNumber(10, 5, score>>2, BLACK);
      break;
    
    case START:
      renderer->drawSprite(14, 40, 25, 7, high_text);
      renderer->drawSprite(40, 40, 34, 7, score_text);
      renderer->drawNumber(75, 40, highscore, BLACK);

      if(start_tick % 50 < 25){
        renderer->drawSprite(20, 60, 119, 11, press_button);
        renderer->drawSprite(40, 75, 79, 11, tostart);  
      }
  }
    renderer->display();
}

void DinoGame::dino_control(GameInput input, SoundPlayer *soundplayer){
  if(dino_is_on_ground){
    if(input.action || input.up){
      dino_vel_y = 50;
      dino_is_on_ground = false;
      //soundplayer->playSoundVolume(jump, 5310);

    }
  }else{
    if(input.down){
      dino_vel_y -= 10;
    }
    dino_vel_y -= gravity;
  }
}

void DinoGame::update_dino(GameInput input, SoundPlayer* soundplayer){

  if(dino_sprite_timer >= 10){
    if(dino_act_sprite == 0){
      dino_act_sprite = 1;
    }else if(dino_act_sprite == 1){
      dino_act_sprite = 0;
    }
    dino_sprite_timer = 0;
  }
  dino_sprite_timer++;


  
  if(game_started_tick >= 15){
    dino_control(input, soundplayer);
  }else{
    game_started_tick++;
  }

  if(dino_vel_y != 0){
    dino_act_sprite = 2;
    dino_pos_y -= dino_vel_y;
    if((dino_pos_y>>SUBPIXEL_SHIFT) + dino_height >= GROUND){
      dino_vel_y = 0;
      dino_pos_y = (GROUND - dino_height)<<SUBPIXEL_SHIFT;
      dino_is_on_ground=  true;
      dino_act_sprite = 0;
    }
  }
}

void DinoGame::update_cacti(){
 
  for(int i = 0; i < MAX_CACTUS; i++){
    cacti[i].pos_x -= speed;
  }
  for(int i = 0; i < 4; i += 2) {
    int first_x = cacti[i].pos_x >> SUBPIXEL_SHIFT;
    int second_x = cacti[i+1].pos_x >> SUBPIXEL_SHIFT;

    bool first_offscreen = first_x < (0 - cacti[i].width);
    bool second_offscreen = !cacti[i+1].is_active || (second_x < (0 - cacti[i+1].width));

    if(first_offscreen && second_offscreen) {
      int other_i = (i == 0) ? 2 : 0;
      int safe_min = 160;
      
      int other_x = cacti[other_i].pos_x >> SUBPIXEL_SHIFT;
      
      if(cacti[other_i].is_active && other_x > 80) {
          safe_min = other_x + 80; 
      }

      int newpos = getRandomRange(safe_min, safe_min + 40);
      
      cacti[i].pos_x = newpos << SUBPIXEL_SHIFT;
      cacti[i].sprite = available_cacti[getRandomRange(0, CACTI)];
      cacti[i].is_active = true;

      if(getRandomBool()) {
        cacti[i+1].pos_x = (newpos + cacti[i].width) << SUBPIXEL_SHIFT;
        cacti[i+1].sprite = available_cacti[getRandomRange(0, CACTI)];
        cacti[i+1].is_active = true;
      } else {
        cacti[i+1].is_active = false;
      }
    }
  }
}

void DinoGame::check_collision(){
  
  int cactus1left = (cacti[0].pos_x>>SUBPIXEL_SHIFT) + cacti[0].hitbox.hitbox_offset_x;
  int cactus1right = (cacti[1].is_active) ? ((cacti[1].pos_x>>SUBPIXEL_SHIFT) + cacti[1].hitbox.hitbox_offset_x + cacti[1].hitbox.width) : ((cacti[0].pos_x>>SUBPIXEL_SHIFT) + cacti[0].hitbox.width + cacti[0].hitbox.hitbox_offset_x);

  int cactus2left = (cacti[2].pos_x>>SUBPIXEL_SHIFT) + cacti[2].hitbox.hitbox_offset_x;
  int cactus2right = (cacti[3].is_active) ? ((cacti[3].pos_x>>SUBPIXEL_SHIFT) + cacti[3].hitbox.hitbox_offset_x + cacti[3].hitbox.width) : ((cacti[2].pos_x>>SUBPIXEL_SHIFT) + cacti[2].hitbox.width + cacti[2].hitbox.hitbox_offset_x);
  
  int dinoleft = (dino_pos_x>>SUBPIXEL_SHIFT) + dino_hitbox.hitbox_offset_x;
  int dinoright = dinoleft + dino_hitbox.width;

  if(((dinoright >= cactus1left) && (dinoleft <=  cactus1right))|| ((dinoright >= cactus2left) && (dinoleft <=  cactus2right))){
    if((dino_pos_y>>SUBPIXEL_SHIFT) + dino_height >= GROUND - 18){
      state = GAME_OVER;
      if((score>>2) > highscore){
        highscore = score>>2;
        nvs_save("DINO", "HIGHSCORE", highscore);
      }
      lose_tick = 0;
    }
  }

}



ChosenGame DinoGame::update(GameInput input, SoundPlayer* soundplayer){

  
  switch(state){
    case PLAYING:

  
      update_grains();
      update_clouds();
      update_cacti();

      update_dino(input, soundplayer);
      check_collision();

      score += 1;

      if(score % 100 == 0) speed++;
      return ChosenGame::DINO_GAME;
    case GAME_OVER:
      lose_tick++;
      if(lose_tick <=15){
        return ChosenGame::DINO_GAME;
      }
      if(input.action || input.up){
        input.action = false;

        init();
        state = PLAYING;
        return ChosenGame::DINO_GAME;
      }else if(input.back){
        return ChosenGame::MENU;
      }else{
        return ChosenGame::DINO_GAME;
      }

    case START:
      start_tick++;

      if(input.action || input.up){
        input.action = false;
        init();
        state = PLAYING;
        return ChosenGame::DINO_GAME;
      }else if(input.back){
        return ChosenGame::MENU;
      }else{
        return ChosenGame::DINO_GAME;
      }
  }
  return ChosenGame::DINO_GAME;
}