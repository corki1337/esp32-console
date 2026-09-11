#include "SoundPlayer.hpp"
#include "audio.h"


SoundPlayer::SoundPlayer(){
    is_muted = false;
    volume = 1;
}


void SoundPlayer::setVolume(float new_volume){
    volume = new_volume;
}


void SoundPlayer::playSound(const int16_t* data, size_t size){
    if(is_muted) return;
    audio_play(data, size*2);
}


void SoundPlayer::playSoundVolume(const int16_t *data, size_t size){

    if(is_muted) return;

    int16_t data_volume[size];
    for(int i = 0; i < size; i++){
        data_volume[i] = (int16_t) (data[i] * volume);
    }

    audio_play(data_volume, size*2);
}