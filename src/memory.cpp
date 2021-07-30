#include <iostream>
#include <filesystem>
#include <fstream>
#include <ctime>
#include "memory.hpp"
#include "bus.hpp"

Memory::Memory(Bus* bus): bus(bus)
{
    this->Reset();
}

Memory::~Memory()
{
    this->bus = nullptr;
}

void Memory::Reset()
{
    this->ROMLoaded = false;
    this->ROMFilepath = "";
    this->ROMSize = 0;
    this->ROMLoadError = "";
    this->snapshotError = "";
    this->snapshotLast = "";
    for (auto& el: this->mem) el = 0;
}

bool Memory::LoadROM(std::string filepath)
{
    std::fstream stream(filepath, std::ios::in|std::ios::binary);
    if (stream) {
        size_t filesize = std::filesystem::file_size(filepath);
        if (filesize > MEMORY_SIZE) {
            std::cerr << "Error: chosen ROM is larger than RAM" << std::endl;
            this->ROMLoadError = "Error: chosen ROM is larger than RAM";
            return 0;
        }
        stream.read((char*)this->mem, filesize);
        this->ROMLoaded = true;
        this->ROMFilepath = filepath;
        this->ROMSize = filesize;
        this->ROMLoadError = "";
        stream.close();
        return 1;
    } else {
        std::cerr << "Error: ROM could not be found or opened" << std::endl;
        this->ROMLoadError = "Error: ROM could not be found or opened";
        return 0;
    }
}

bool Memory::SnapshotRAM(std::string filepath)
{
    std::fstream stream(filepath, std::ios::out|std::ios::binary);
    if (stream) {
        stream.write((char*)this->mem, MEMORY_SIZE);
        this->snapshotError = "";
        std::time_t res = std::time(nullptr);
        this->snapshotLast = std::ctime(&res);
        stream.close();
        return 1;
    } else {
        std::cerr << "Error: could not write RAM contents to filepath " << filepath << std::endl;
        this->snapshotError = "Error: could not write RAM contents to filepath " + filepath;
        return 0;
    }
}