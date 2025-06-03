//
// Created by albert on 5/16/25.
//

#include "chip.h"

#include <iostream>
#include <fstream>
#include <format>
#include <memory>
#include <sstream>

// This is all the implementation for the chip-8 hardware
namespace Chip8 {
    Chip::Chip() :    // initialize list
    registers(std::make_unique<std::array<uint8_t, 16>>()),
    memory(std::make_unique<std::array<uint8_t, 4096>>()),
    stack(std::make_unique<std::array<uint16_t, 16>>()),
    fonts {{
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80, // F
    }}  // DI from this
    {
        init_counters();
        init_timers(0, 0);    // use default argument values 0, 0
        init_random_generator();
        load_fonts_in_memory();
    }

    int Chip::init_counters() {
        this->index_reg = 0x000; // aka i (stores memory address by rom)
        this->program_ctr = 0x200; // program line counter
        this->stack_ptr = 0x000; // stack address pointer

        return 0;
    }

    int Chip::init_timers(uint8_t delay_time = 0, uint8_t sound_time = 0) {
        this->delay_timer = delay_time;
        this->sound_timer = sound_time;

        return 0;
    }

    void Chip::init_instr_dispatcher() {
        instr_dispatcher = std::make_shared<Instructions>(shared_from_this());
    }

    void Chip::init_gfx() {
        gfx = std::make_shared<std::array<uint8_t, 64 * 32>>();
    }

    void Chip::init_random_generator() {
        uniform_dist = std::uniform_int_distribution<uint8_t>{ 0, 255u };
        random_engine = std::default_random_engine{};
    }

    int Chip::load_rom(std::ifstream *file_stream) {
        std::streamsize file_size = file_stream->tellg();
        std::vector<char> buffer(file_size);

        file_stream->seekg(0, std::ios::beg);
        if (!file_stream->read(buffer.data(), file_size))
            throw std::runtime_error("Read failed");

        std::copy(
            buffer.begin(),
            buffer.end(),
            memory->begin() + rom_start_addr
        );
        set_rom_loaded(true);

        return 0;
    }

    bool Chip::load_fonts_in_memory(std::string start_address_hex) {
        // Stores 5 bytes * 16 fonts = 80 bytes inside [0x050, 0x09F]
        int start_address_int;
        std::stringstream ss;
        ss << std::hex << start_address_hex;
        ss >> start_address_int;
        if (start_address_int > 432) {
            std::cerr << "start_address is too high to fit in between 0x000 and 0x200";
            return false;
        }
        for (int i = 0; i < 80; i++) {
            (*this->memory)[start_address_int + i] = this->fonts[i];
        }
        return true;
    }

    bool Chip::get_rom_loaded() {
        return rom_loaded;
    }

    void Chip::set_rom_loaded(bool status) {
        rom_loaded = status;
    }

    int Chip::decrement_timers() {
        if (delay_timer < 0 || sound_timer < 0) return -1;
        if (delay_timer > 0) delay_timer--;
        if (sound_timer > 0) sound_timer--;
        return 0;
    }

    void Chip::set_sound_timer(uint8_t time) {
        sound_timer = time;
    }

    int Chip::cycle() {
        // validation
        if (program_ctr + 1 >= memory->size()) {
            std::cout << "Resetting program_ctr" << std::endl;
            program_ctr = rom_start_addr;
            // throw std::out_of_range("PCOutOfBoundsException: Crashed Program\n");
        }

        // fetch two bytes
        uint8_t high = memory->at(program_ctr);
        uint8_t low  = memory->at(program_ctr + 1);

        // execute
        uint16_t opcode = ((uint16_t) high << 8) | low; // combine two byte using bitwise
        instr_dispatcher->interpret_opcode(opcode);

        // move relevant counters and timers
        program_ctr += 2;
        return 0;
    }

    uint8_t Chip::get_random_number() {
        return uniform_dist(random_engine);
    }
}