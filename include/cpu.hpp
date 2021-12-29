#pragma once

/**
 * The Bytepusher's CPU, powered by a OISC (one instruction-set computer)
 * called ByteByteJump. During an instruction, the CPU reads in three 3-byte values
 * from memory (pointed to by the PC, program counter) A, B, and C. It then performs two steps,
 * 1. copy the value from memory[A] to memory[B],
 * 2. set the PC to C.
 */

#include <iostream>
#include "memory.hpp"

class Bus;

class CPU {
public:
    CPU(Bus* bus);
    ~CPU();

    void Step();
    void Reset();

private:
    Bus* bus;
    uint8_t* pc;
};