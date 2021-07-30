#include <iostream>
#include "memory.hpp"
#include "audio.hpp"
#include "bus.hpp"

Audio::Audio(Bus* bus): bus(bus), spec({}), device(0)
{   
    SDL_AudioSpec spec = {};
    spec.freq = 15360;
    spec.format = AUDIO_U8;
    spec.channels = 1;
    spec.samples = 256;

    this->device = SDL_OpenAudioDevice(nullptr, 0, &spec, &this->spec, 0);
    if (!this->device) {
        std::cerr << "I had trouble creating the audio device!\nSDL error: " << SDL_GetError() << std::endl;
        exit(1);
    }
    SDL_PauseAudioDevice(this->device, 0);
}

Audio::~Audio()
{
    if (this->device) {
        SDL_PauseAudioDevice(this->device, 1);
        SDL_CloseAudioDevice(this->device);
        this->device = 0;
    }
    this->spec = {};
    this->bus = nullptr;
}

void Audio::Play()
{
    uint32_t location = (uint32_t)this->bus->memory->mem[6] << 16 | this->bus->memory->mem[7] << 8;
    
    if (SDL_QueueAudio(this->device, this->bus->memory->mem + location, 256)) {
        std::cerr << "I had trouble playing audio!\nSDL error: " << SDL_GetError() << std::endl;
    }
}