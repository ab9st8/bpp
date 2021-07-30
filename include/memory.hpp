#pragma once

/**
 * The memory of the Bytepusher. 16 MiB (0x1000008 bytes).
 * The memory map is located at the beginning of memory and
 * looks like this:
 * 
 * memory (byte) | description
 * --------------|----------------
 * 0, 1          | Keyboard state, if key X is
 *               | pressed then bit X is on
 *               |
 * 2, 3, 4       | The program counter starts here
 *               |
 * 5             | Graphics block location. A value
 *               | of ZZ means color of pixel at coordinate (XX, YY)
 *               | is at ZZYYXX
 *               |
 * 6, 7          | Sound block location. A value of XXYY
 *               | means audio sample ZZ is at address XXYY
 * -------------------------------
 * The byte ordering used by Bytepusher is big-endian.
 */

#include <stdlib.h>
#include <string>
class Bus;

class Memory {
public:
    Memory(Bus* bus);
    ~Memory();

    static const size_t MEMORY_SIZE = 0x1000008;
    uint8_t             mem[MEMORY_SIZE];
    bool                ROMLoaded;
    std::string         ROMFilepath;
    size_t              ROMSize;
    std::string         ROMLoadError;
    std::string         snapshotError;
    std::string         snapshotLast;


    void Reset();
    bool LoadROM(std::string filepath);
    bool SnapshotRAM(std::string filepath);
private:
    Bus* bus;
};