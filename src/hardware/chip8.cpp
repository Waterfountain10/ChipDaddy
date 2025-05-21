//
// Created by albert on 5/16/25.
//

#include "chip8.h"
#include <iostream>
#include <memory>

// This is all the implementation for the chip-8 hardware
namespace Chip8 {
    Chip8::Chip8() :    // initialize list
    registers(std::make_unique<std::array<uint8_t, 16>>()),
    memory(std::make_unique<std::array<uint8_t, 4096>>()),
    stack(std::make_unique<std::array<uint16_t, 16>>()),
    gfx(std::make_shared<std::array<uint8_t, 64 * 32>>())
    {
        this->init_counters();
        this->init_timers(0, 0);    // use default argument values 0, 0
    }

    int Chip8::init_counters() {
        this->index_reg = 0x000;
        this->program_ctr = 0x000;
        this->stack_ptr = 0x000;    // init to final memory addr

        return 0;
    }

    int Chip8::init_timers(uint8_t delay_time = 0, uint8_t sound_time = 0) {
        this->delay_timer = delay_time;
        this->sound_timer = sound_time;

        return 0;
    }

}