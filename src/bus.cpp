#include "bus.hpp"

Bus::Bus(): memory(nullptr), cpu(nullptr), display(nullptr), audio(nullptr), input(nullptr)
{}

Bus::~Bus()
{
    this->memory = nullptr;
    this->cpu = nullptr;
    this->display = nullptr;
    this->audio = nullptr;
    this->input = nullptr;
}