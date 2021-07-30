#include "input.hpp"
#include "memory.hpp"
#include "bus.hpp"

Input::Input(Bus* bus): bus(bus) {}

Input::~Input()
{
    this->bus = nullptr;
}

void Input::HandleKeyDown(SDL_Keycode keycode)
{
    switch (keycode)
    {
    case SDLK_1: this->SetKeyBit(1); break;
	case SDLK_2: this->SetKeyBit(2); break;
	case SDLK_3: this->SetKeyBit(3); break;
    case SDLK_4: this->SetKeyBit(12); break;
    case SDLK_q: this->SetKeyBit(4); break;
    case SDLK_w: this->SetKeyBit(5); break;
    case SDLK_e: this->SetKeyBit(6); break;
    case SDLK_r: this->SetKeyBit(13); break;
    case SDLK_a: this->SetKeyBit(7); break;
    case SDLK_s: this->SetKeyBit(8); break;
    case SDLK_d: this->SetKeyBit(9); break;
    case SDLK_f: this->SetKeyBit(14); break;
    case SDLK_z: this->SetKeyBit(10); break;
    case SDLK_x: this->SetKeyBit(0); break;
    case SDLK_c: this->SetKeyBit(11); break;
    case SDLK_v: this->SetKeyBit(15); break;
    
    default: break;
    }
}

void Input::HandleKeyUp(SDL_Keycode keycode)
{
    switch (keycode)
    {
    case SDLK_1: this->UnsetKeyBit(1); break;
    case SDLK_2: this->UnsetKeyBit(2); break;
    case SDLK_3: this->UnsetKeyBit(3); break;
    case SDLK_4: this->UnsetKeyBit(12); break;
    case SDLK_q: this->UnsetKeyBit(4); break;
    case SDLK_w: this->UnsetKeyBit(5); break;
    case SDLK_e: this->UnsetKeyBit(6); break;
    case SDLK_r: this->UnsetKeyBit(13); break;
    case SDLK_a: this->UnsetKeyBit(7); break;
    case SDLK_s: this->UnsetKeyBit(8); break;
    case SDLK_d: this->UnsetKeyBit(9); break;
    case SDLK_f: this->UnsetKeyBit(14); break;
    case SDLK_z: this->UnsetKeyBit(10); break;
    case SDLK_x: this->UnsetKeyBit(0); break;
    case SDLK_c: this->UnsetKeyBit(11); break;
    case SDLK_v: this->UnsetKeyBit(15); break;
    
    default: break;
    }
}

void Input::SetKeyBit(uint8_t bit)
{
    uint32_t* keyboard = (uint32_t*)this->bus->memory->mem;
    if (bit < 8) *keyboard |= (1 << (bit + 8));
    else *keyboard |= (1 << (bit - 8));
}

void Input::UnsetKeyBit(uint8_t bit)
{
    uint32_t* keyboard = (uint32_t*)this->bus->memory->mem;
    if (bit < 8) *keyboard &= ~(1 << (bit + 8));
    else *keyboard &= ~(1 << (bit - 8));
}