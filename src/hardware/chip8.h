//
// Created by Alb33rt on 5/16/25.
//

#ifndef CHIP8_H
#define CHIP8_H

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

namespace Chip8 {
    class Chip8 {
    public:
        static const uint16_t rom_start_addr = 0x200;

        const std::unique_ptr<std::array<uint8_t, 16>> registers;  //uint8_t registers[16];
        const std::unique_ptr<std::array<uint8_t, 4096>> memory;     //uint8_t memory[4096];
        const std::unique_ptr<std::array<uint16_t, 16>> stack;     //uint16_t stack[16];
        // TODO: Whether to make these dynamic using stdlib
        uint16_t index_reg;
        uint16_t program_ctr;
        uint8_t stack_ptr;
        uint8_t delay_timer;
        uint8_t sound_timer;
        const std::shared_ptr<std::array<uint8_t, 64 * 32>> gfx; //uint8_t gfx[64 * 32];
        std::array<uint8_t, 80> fonts;

        // Do not reference platform with shared_ptr

        explicit Chip8();
        int init_counters();
        int init_timers(uint8_t delay_time, uint8_t sound_time);
        bool load_fonts_in_memory(std::string start_address = "050");
        // ~Chip8();

        int load_rom(std::ifstream *file_stream);

    private:
        //const std::unique_ptr<std::array<>>
    };
} // Chip8

#endif //CHIP8_H
