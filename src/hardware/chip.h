//
// Created by Alb33rt on 5/16/25.
//

#ifndef CHIP8_H
#define CHIP8_H

#include <array>
#include <cstdint>
#include <memory>
#include <vector>

#include "instructions.h"

namespace Chip8 {

    class Instructions; // avoid circular declarations

    class Chip : public std::enable_shared_from_this<Chip> {
    public:
        static const uint16_t rom_start_addr = 0x200;

        const std::unique_ptr<std::array<uint8_t, 16>> registers;  //uint8_t registers[16];
        const std::unique_ptr<std::array<uint8_t, 4096>> memory;     //uint8_t memory[4096];
        const std::unique_ptr<std::array<uint16_t, 16>> stack;     //uint16_t stack[16];
        const std::shared_ptr<std::array<uint8_t, 64 * 32>> gfx; //uint8_t gfx[64 * 32];
        std::array<uint8_t, 80> fonts;

        std::shared_ptr<Instructions> instr_dispatcher; // lifetime is managed by the hardware
        // Do not reference platform as it is abstraction layer

        explicit Chip();
        ~Chip() = default;
        int init_counters();
        int init_timers(uint8_t delay_time, uint8_t sound_time);
        void init_instr_dispatcher();

        bool load_fonts_in_memory(std::string start_address = "050");
        bool get_rom_loaded();
        int load_rom(std::ifstream *file_stream);

        int decrement_timers();

        int cycle();    // main loop

    private:
        bool rom_loaded = false;

        // TODO: Whether to make these dynamic using stdlib
        uint16_t index_reg;
        uint16_t program_ctr;
        uint8_t stack_ptr;
        uint8_t delay_timer;
        uint8_t sound_timer;

        void set_rom_loaded(bool status);
    };
} // Chip8

#endif //CHIP8_H
