#pragma once

/**
 * A simple "system bus" which allows for interconnection between
 * components, saving us passing pointers or references to each
 * other every function call.
 */

class CPU;
class Display;
class Audio;
class Memory;
class Input;

class Bus {
public:
    Bus();
    ~Bus();

    Memory*  memory;
    CPU*     cpu;
    Display* display;
    Audio*   audio;
    Input*   input;
};