#include "SoundPlayer.hpp"
#include "audio.h"


SoundPlayer::SoundPlayer(){
    is_muted = false;
    volume = 1.0f;
}


void SoundPlayer::setVolume(float new_volume){
    volume = new_volume;
}


void SoundPlayer::playSound(const int16_t* data, size_t size){
    if(is_muted) return;
    audio_play(data, size/2, 1.0f);
}


void SoundPlayer::playSoundVolume(const int16_t *data, size_t size){

    if(is_muted) return;


    audio_play(data, size/2, volume);
}