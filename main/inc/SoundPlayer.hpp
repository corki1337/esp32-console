#pragma once

#include "stdint.h"



class SoundPlayer{
public:
    SoundPlayer();

    void setVolume(float volume);

    void playSound(const int16_t *data, size_t size);

    void playSoundVolume(const int16_t *data, size_t size);

    void setmute(bool mute);

private:
    bool is_muted;
    float volume;

};