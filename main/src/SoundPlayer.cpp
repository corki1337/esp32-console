#include "SoundPlayer.hpp"
#include "audio.h"
#include "nvsmem.h"


SoundPlayer::SoundPlayer(){

    is_muted = (nvs_read("Settings", "ismuted") >= 1) ? true : false;
    volume = (nvs_read("Settings", "volume") >= 1) ? 1.0f : nvs_read("Settings", "volume");
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

void SoundPlayer::setmute(bool mute){
    is_muted = mute;
}