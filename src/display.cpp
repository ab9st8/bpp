#include <iostream>
#include "memory.hpp"
#include "display.hpp"
#include "bus.hpp"

Display::Display(Bus* bus, SDL_Renderer* ren): bus(bus)
{
    this->Reset();
    this->displayTexture = SDL_CreateTexture(ren, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, width, height);
	if (!displayTexture) {
        std::cerr << "Texture creation error: " << SDL_GetError() << std::endl;
		exit(1);
	}
	this->PrepareColorPalette();
}

void Display::Reset()
{
	for (auto& el: this->display) el = {0, 0, 0, 255};
}

void Display::PrepareColorPalette()
{
	size_t i = 0;

	for (size_t r = 0; r < 256; r += 0x33) {
		for (size_t g = 0; g < 256; g += 0x33) {
			for (size_t b = 0; b < 256; b += 0x33) {
				this->colorPalette[i].r = r;
				this->colorPalette[i].g = g;
				this->colorPalette[i].b = b;
				
				i++;
			}
		}
	}
}

void Display::UpdateTexture()
{
	uint32_t location = (uint32_t)this->bus->memory->mem[5];

	for (size_t y = 0; y < 256; y++) {
		for (size_t x = 0; x < 256; x++) {
			this->display[(y * 256) + x] = this->colorPalette[this->bus->memory->mem[location << 16 | y << 8 | x ]];
		}
	}

	SDL_UpdateTexture(this->displayTexture, NULL, display, width * 4);
}

Display::~Display()
{
    SDL_DestroyTexture(this->displayTexture);
}