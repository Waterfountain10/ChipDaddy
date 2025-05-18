//
// Created by Alb33rt on 5/16/25.
//

#ifndef CHIP8_H
#define CHIP8_H
#include <cstdint>
#include <vector>

namespace Chip8 {
    class Chip8 {
    public:
        std::vector<uint8_t> registers; //uint8_t registers[16];
        std::vector<uint8_t> memory; //uint8_t memory[4096];
        uint16_t index_reg;
        uint16_t program_ctr;
        std::vector<uint16_t> stack; //uint16_t stack[16];
        uint8_t stack_ptr;
        uint8_t delay_timer;
        uint8_t sound_timer;
        std::vector<std::vector<uint8_t>> gfx; //uint8_t gfx[64 * 32];
    private:

    };
} // Chip8

#endif //CHIP8_H
