//
// Created by Alb33rt on 5/16/25.
//

#ifndef CHIP8_H
#define CHIP8_H

#include <array>
#include <cstdint>
#include <random>
#include <memory>
#include <vector>

#include "instructions.h"

namespace Chip8 {
    const std::string FONT_START_ADDRESS = "050";
    class Instructions; // avoid circular declarations

    class Chip : public std::enable_shared_from_this<Chip> {
    public:
        static const uint16_t rom_start_addr = 0x200;

        const std::unique_ptr<std::array<uint8_t, 16>> registers;  //uint8_t registers[16];
        const std::unique_ptr<std::array<uint8_t, 4096>> memory;     //uint8_t memory[4096];
        const std::unique_ptr<std::array<uint16_t, 16>> stack;     //uint16_t stack[16];
        std::shared_ptr<std::array<std::array<uint8_t, 32>, 64>> gfx; //uint8_t gfx[64][32];
        std::array<uint8_t, 80> fonts;

        std::shared_ptr<Instructions> instr_dispatcher; // lifetime is managed by the hardware
        // Do not reference platform as it is abstraction layer

        uint16_t index_reg;
        uint16_t program_ctr;
        uint8_t stack_ptr;
        uint8_t delay_timer;
        uint8_t sound_timer;
        bool waiting_for_key;
        uint8_t waiting_reg;

        uint16_t font_start_address;

        explicit Chip();
        ~Chip() = default;
        int init_counters();
        int init_timers(uint8_t delay_time, uint8_t sound_time);
        void init_instr_dispatcher();
        void init_gfx();
        void init_waiting();

        bool load_fonts_in_memory(std::string start_address = FONT_START_ADDRESS);
        bool get_rom_loaded();
        int load_rom(std::ifstream *file_stream);

        void set_sound_timer(uint8_t time);

        int decrement_timers();

        int cycle();    // main loop

        // std::shared_ptr<std::array<std::array<uint8_t, 32>, 64>> get_gfx() const {
        //     return gfx;
        // }

        uint8_t get_random_number();

        bool is_waiting_for_key();

        void set_waiting_register(uint8_t reg);

        void complete_key_wait(uint8_t key);

    private:
        std::uniform_int_distribution<uint8_t> uniform_dist;
        std::default_random_engine random_engine;
        bool rom_loaded = false;

        void init_random_generator();

        void set_rom_loaded(bool status);
    };
} // Chip8

#endif //CHIP8_H
