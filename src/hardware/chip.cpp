//
// Created by albert on 5/16/25.
//

#include "chip.h"

#include <iostream>
#include <fstream>
#include <sstream>

// This is all the implementation for the chip-8 hardware
namespace Chip8 {
    /**
     * @brief Constructs the CHIP-8 emulator hardware state.
     *
     * Initializes registers, memory, stack, key state tracking, and loads font data.
     * Sets up internal counters, timers, random generator, and key-wait state.
     */
    Chip::Chip() :
    registers(std::make_unique<std::array<uint8_t, 16>>()),
    memory(std::make_unique<std::array<uint8_t, 4096>>()),
    stack(std::make_unique<std::array<uint16_t, 16>>()),
    key_states(std::make_unique<std::set<uint8_t>>()),
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
    }}
    {
        init_counters();
        init_timers(0, 0);    // use default argument values 0, 0
        init_random_generator();
        load_fonts_in_memory();
        init_waiting();
    }

    // INITIALIZERS

    /**
     * @brief Initializes the index register, program counter, and stack pointer.
     *
     * @return Always returns 0.
     */
    int Chip::init_counters() {
        this->index_reg = 0x000; // aka i (stores memory address by rom)
        this->program_ctr = 0x200; // program line counter
        this->stack_ptr = 0x000; // stack address pointer

        return 0;
    }

    /**
     * @brief Initializes delay and sound timers.
     *
     * @param delay_time Initial value for delay timer (default 0).
     * @param sound_time Initial value for sound timer (default 0).
     * @return Always returns 0.
     */
    int Chip::init_timers(uint8_t delay_time = 0, uint8_t sound_time = 0) {
        this->delay_timer = delay_time;
        this->sound_timer = sound_time;

        return 0;
    }

    /**
     * @brief Initializes the opcode instruction dispatcher.
     *
     * Creates an Instructions instance bound to this Chip.
     */
    void Chip::init_instr_dispatcher() {
        instr_dispatcher = std::make_shared<Instructions>(shared_from_this());
    }

    /**
     * @brief Initializes the graphics buffer.
     *
     * Allocates a 64x32 array for pixel state.
     */
    void Chip::init_gfx() {
        gfx = std::make_shared<std::array<std::array<uint8_t, 32>, 64>>();
    }

    /**
     * @brief Initializes the random number generator.
     *
     * Configures uniform distribution over 0-255 and default engine.
     */
    void Chip::init_random_generator() {
        uniform_dist = std::uniform_int_distribution<uint8_t>{ 0, 255u };
        random_engine = std::default_random_engine{};
    }

    /**
     * @brief Resets the key-waiting state.
     *
     * waiting_for_key set to false and waiting_reg reset.
     */
    void Chip::init_waiting() {
        this->waiting_for_key = false;
        this->waiting_reg = 0xFF;
    }

    // HARDWARE FUNCTIONS

    /**
     * @brief Loads a CHIP-8 ROM from file into memory starting at 0x200.
     *
     * @param file_stream Pointer to an open ifstream for the ROM file.
     * @throws std::runtime_error if file read fails.
     * @return 0 on success.
     */
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

    /**
     * @brief Loads font sprite data into memory.
     *
     * Each font character is 5 bytes, total 80 bytes written at font_start_address.
     *
     * @param start_address_hex Hex string specifying where to load fonts.
     * @return True on successful load; false if address invalid.
     */
    bool Chip::load_fonts_in_memory(std::string start_address_hex) {
        // Set start_address_int by parsing string
        std::stringstream ss;
        ss << std::hex << start_address_hex;
        ss >> font_start_address;

        if (font_start_address > 432) {
            std::cerr << "start_address is too high to fit in between 0x000 and 0x200";
            return false;
        }

        for (int i = 0; i < 80; i++) {
            (*this->memory)[font_start_address + i] = this->fonts[i];
        }
        return true;
    }

    /**
     * @brief Checks if a ROM has been successfully loaded.
     *
     * @return True if ROM loaded, false otherwise.
     */
    bool Chip::get_rom_loaded() {
        return rom_loaded;
    }

    /**
     * @brief Sets the ROM loaded status.
     *
     * @param status Boolean flag indicating ROM load state.
     */
    void Chip::set_rom_loaded(bool status) {
        rom_loaded = status;
    }

    /**
     * @brief Decrements the delay and sound timers if above zero.
     *
     * @return 0 on success; -1 if timers underflow.
     */
    int Chip::decrement_timers() {
        if (delay_timer < 0 || sound_timer < 0) return -1;
        if (delay_timer > 0) delay_timer--;
        if (sound_timer > 0) sound_timer--;
        return 0;
    }

    /**
     * @brief Sets the sound timer to a specific value.
     *
     * @param time New sound timer value.
     */
    void Chip::set_sound_timer(uint8_t time) {
        sound_timer = time;
    }

    /**
     * @brief Performs one CPU cycle: fetch, decode, execute.
     *
     * @throws std::out_of_range if program counter exceeds memory.
     * @return 0 on successful cycle.
     */
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

    /**
     * @brief Generates a random 8-bit value using uniform distribution.
     *
     * @return A random uint8_t in range [0,255].
     */
    uint8_t Chip::get_random_number() {
        return uniform_dist(random_engine);
    }

    void Chip::add_key_state(uint8_t key) {
        if (key <= 15) {    // uint8_t always >= 0
            key_states->insert(key);
        }
    }

    int Chip::remove_key_state(uint8_t key) {
        return key_states->erase(key);
    }

    /**
     * @brief Checks if a key is pressed.
     *
     * Used for E-Codes for skipping if key pressed or not.
     * Comparing chip 8 key (virtual keyboard)
     * NOT the SDL event key type (real keyboard)
     *
     * @param key
     * `
     * @return True if chip8 key is pressed
     */
    bool Chip::is_key_pressed(uint8_t key) {
        return key_states->count(key) > 0;
    }

    bool Chip::is_waiting_for_key() {
        return waiting_for_key;
    }

    void Chip::set_waiting_register(uint8_t reg) {
        waiting_for_key = true;
        waiting_reg = reg;
    }

    /**
     * @brief Completes a key wait by storing the pressed key in Vx.
     *
     * Releases waiting state and writes key value.
     *
     * @param key Index of the key that was pressed.
     */
    void Chip::complete_key_wait(uint8_t key) {
        registers->at(waiting_reg) = key; // store the value of released key in Vx

        waiting_for_key = false; // so run_frame resumes cycle()
        waiting_reg = 0xFF; // default value
    }
}