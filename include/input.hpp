#pragma once

/**
 * The Bytepusher's controller consists of 16 keys
 * in a 4x4 keyboard labelled 0-F;
 * 
 * 1 (1) || 2 (2) || 3 (3) || C (4)
 * --------------------------------
 * 4 (Q) || 5 (W) || 6 (E) || D (R)
 * --------------------------------
 * 7 (A) || 8 (S) || 9 (D) || E (F)
 * --------------------------------
 * A (Z) || 0 (X) || B (C) || C (V)
 * 
 * whose states correspond to those of their respective
 * bits in the first two bytes of memory. The actual
 * keyboard state can be seen in the debug window.
 */

#include <SDL2/SDL.h>

class Bus;

class Input {
public:
    Input(Bus* bus);
    ~Input();

    void HandleKeyDown(SDL_Keycode keycode);
    void HandleKeyUp(SDL_Keycode keycode);
    void SetKeyBit(uint8_t bit);
    void UnsetKeyBit(uint8_t bit);

private:
    Bus* bus;
};