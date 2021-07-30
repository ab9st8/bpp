#pragma once

/**
 * The Bytepusher's audio device. 8-bit mono sound with 256 samples
 * per frame, their location in memory being indicated by bytes 6
 * and 7 in the memory map.
 * With the audio I largely helped myself with David Jolly's (majestic53)
 * implementation `bpvm` which you can find on his GitHub. Thanks David!
 */

#include <stdlib.h>
#include <SDL2/SDL.h>

class Bus;

class Audio {
public:
    Audio(Bus* bus);
    ~Audio();

    void Play();
private:
    Bus* bus;
    SDL_AudioSpec spec;
    SDL_AudioDeviceID device;
};