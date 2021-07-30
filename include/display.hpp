#pragma once

/**
 * The Bytepusher has a display of dimensions 256 x 256,
 * 1 byte per pixel (we scale it to 512x512 anyway). That makes
 * the display data 65 536 bytes large. The location of that display
 * data is indicated by byte 5 in the memory map.
 * The color palette has 216 colors, leaving the remaining 256-216=40 colors
 * simply black.
 * Each color is a combination of red, green, and blue, of which each
 * can have an intensity from 0 to 5. That makes the increment 0x33:
 * 
 * ---------------------------------------------------------
 * intensity || 0    || 1    || 2    || 3    || 4    || 5
 * value     || 0x00 || 0x33 || 0x66 || 0x99 || 0xCC || 0xFF
 * ---------------------------------------------------------
 */

#include <SDL2/SDL.h>
class Bus;

class Display {
public:
    Display(Bus* bus, SDL_Renderer* ren);
    ~Display();

    void PrepareColorPalette();
    void UpdateTexture();
    void Reset();

    SDL_Texture* displayTexture;

    static const int width = 256;
    static const int height = 256;
    SDL_Color display[width * height];
    
private:
    Bus* bus;
    SDL_Color colorPalette[256];
};