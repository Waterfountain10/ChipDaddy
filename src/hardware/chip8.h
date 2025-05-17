//
// Created by Alb33rt on 5/16/25.
//

#ifndef CHIP8_H
#define CHIP8_H
#include <cstdint>

namespace chip {
    class Chip8 {
    public:
        uint8_t registers[16];
        uint8_t memory[4096];
        uint16_t index_reg;
        uint16_t program_counter;
        uint16_t stack[16];
        uint8_t stack_pointer;

        uint8_t delay_timer;
        uint8_t sound_timer;

    private:

    };
} // chip

#endif //CHIP8_H
