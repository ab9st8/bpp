#include "cpu.hpp"
#include "bus.hpp"
#include "memory.hpp"

// i'm sorry but there's no way i'm typing this every time i want to access memory
// (which is a lot of times in this file)
#define MEMORY this->bus->memory->mem

CPU::CPU(Bus* bus): bus(bus)
{
    this->Reset();
}

CPU::~CPU()
{
    this->bus = nullptr;
}

void CPU::Step()
{
    uint32_t destination = (uint32_t)this->pc[3] << 16 | (uint32_t)this->pc[4] << 8 | this->pc[5];
    uint32_t source =      (uint32_t)this->pc[0] << 16 | (uint32_t)this->pc[1] << 8 | this->pc[2];
    uint32_t jump =        (uint32_t)this->pc[6] << 16 | (uint32_t)this->pc[7] << 8 | this->pc[8];

    MEMORY[destination] = MEMORY[source];
    this->pc = MEMORY + jump;
}

void CPU::Reset()
{
    this->pc = MEMORY + ((uint32_t)MEMORY[2]<<16 | (uint32_t)MEMORY[3]<<8 | MEMORY[4]);
}

void CPU::setKeyBit(uint8_t index)
{
    uint32_t* keyboard = (uint32_t*)MEMORY;
    if (index < 8) *keyboard |= 1 << (index + 8);
    else *keyboard |= 1 << (index - 8);
}

void CPU::clearKeyBit(uint8_t index)
{
    uint32_t* keyboard = (uint32_t*)MEMORY;
    if (index < 8) *keyboard &= ~(1 << (index + 8));
    else *keyboard &= ~(1 << (index - 8));
}

#undef MEMORY
// goodbye