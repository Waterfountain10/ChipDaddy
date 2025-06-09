//
// Created by albert on 5/16/25.
//

#include "instructions.h"

#include <chrono>
#include <map>
#include <SDL_events.h>
#include <SDL_timer.h>

namespace Chip8 {
    // public
    Instructions::Instructions(std::shared_ptr<Chip> chip8_instance) :
    chip8_(chip8_instance)
    {  // constructor
        init_dispatch_table();
    }

    int Instructions::interpret_opcode(uint16_t p_opcode) {
        // static
        opcode = p_opcode;
        // Lock weak_ptr and operate using weak_ptr
        if (auto chip8_ptr = chip8_.lock()) {
            (this->*dispatch_table[(opcode & 0xF000u) >> 12u])(chip8_ptr); // executes specific
            // instruction
            chip8_ptr.reset();
        }
        return 0;
    }

    // private
    void Instructions::init_dispatch_table() {  // TODO: Potential optimization -> Make Hash Map
    // for quick access instead
        zero_dispatch_table = std::array<Handler, ZERO_OPS>{};
        zero_dispatch_table.fill(&Instructions::OP_NULL);
        zero_dispatch_table[0x0] = &Instructions::OP_00E0;
        zero_dispatch_table[0xE] = &Instructions::OP_00EE;

        eight_dispatch_table = std::array<Handler, EIGHT_OPS>{};
        eight_dispatch_table.fill(&Instructions::OP_NULL);
        eight_dispatch_table[0x0] = &Instructions::OP_8XY0;
        eight_dispatch_table[0x1] = &Instructions::OP_8XY1;
        eight_dispatch_table[0x2] = &Instructions::OP_8XY2;
        eight_dispatch_table[0x3] = &Instructions::OP_8XY3;
        eight_dispatch_table[0x4] = &Instructions::OP_8XY4;
        eight_dispatch_table[0x5] = &Instructions::OP_8XY5;
        eight_dispatch_table[0x6] = &Instructions::OP_8XY6;
        eight_dispatch_table[0x7] = &Instructions::OP_8XY7;
        eight_dispatch_table[0xE] = &Instructions::OP_8XYE;

        e_dispatch_table = std::array<Handler, E_OPS>{};
        e_dispatch_table.fill(&Instructions::OP_NULL);
        e_dispatch_table[0x1] = &Instructions::OP_EXA1;
        e_dispatch_table[0xE] = &Instructions::OP_EX9E;

        f_dispatch_table = std::array<Handler, F_OPS>{};
        f_dispatch_table.fill(&Instructions::OP_NULL);
        f_dispatch_table[0x07] = &Instructions::OP_FX07;
        f_dispatch_table[0x0A] = &Instructions::OP_FX0A;
        f_dispatch_table[0x15] = &Instructions::OP_FX15;
        f_dispatch_table[0x18] = &Instructions::OP_FX18;
        f_dispatch_table[0x1E] = &Instructions::OP_FX1E;
        f_dispatch_table[0x29] = &Instructions::OP_FX29;
        f_dispatch_table[0x33] = &Instructions::OP_FX33;
        f_dispatch_table[0x55] = &Instructions::OP_FX55;
        f_dispatch_table[0x65] = &Instructions::OP_FX65;

        dispatch_table = std::array<Handler, DISPATCH_SIZE>{};
        dispatch_table[0x0] = &Instructions::OP_0;
        dispatch_table[0x1] = &Instructions::OP_1NNN;
        dispatch_table[0x2] = &Instructions::OP_2NNN;
        dispatch_table[0x3] = &Instructions::OP_3XNN;
        dispatch_table[0x4] = &Instructions::OP_4XNN;
        dispatch_table[0x5] = &Instructions::OP_5XY0;
        dispatch_table[0x6] = &Instructions::OP_6XNN;
        dispatch_table[0x7] = &Instructions::OP_7XNN;
        dispatch_table[0x8] = &Instructions::OP_8;
        dispatch_table[0x9] = &Instructions::OP_9XY0;
        dispatch_table[0xA] = &Instructions::OP_ANNN;
        dispatch_table[0xB] = &Instructions::OP_BNNN;
        dispatch_table[0xC] = &Instructions::OP_CXNN;
        dispatch_table[0xD] = &Instructions::OP_DXYN;
        dispatch_table[0xE] = &Instructions::OP_E;
        dispatch_table[0xF] = &Instructions::OP_F;
    }

    // TODO: Add all the remaining instructions
    void Instructions::OP_0(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        (this->*zero_dispatch_table[(opcode & 0x000Fu)])(chip8_ptr);     // masks last bit and executes
    }

    void Instructions::OP_00E0(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_00EE(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        chip8_ptr->stack_ptr--;
        chip8_ptr->program_ctr = chip8_ptr->stack->at(chip8_ptr->stack_ptr);
    }

    void Instructions::OP_1NNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint16_t addr = opcode & 0x0FFF;
        chip8_ptr->program_ctr = addr;
    }

    void Instructions::OP_2NNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        chip8_ptr->stack->at(chip8_ptr->stack_ptr) = 1;
        chip8_ptr->stack_ptr++;

        uint16_t addr = opcode & 0x0FFFu;
        chip8_ptr->program_ctr = addr;
    }

    void Instructions::OP_3XNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_4XNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg = (opcode & 0x0F00u) >> 8u;     // Masks third digit then shifts to keep
        uint8_t byte = (opcode & 0x00FFu);    // Masks bottom 8-bits

        if (chip8_ptr->registers->at(reg) != byte) {
            chip8_ptr->program_ctr += 2;
        }
    }

    void Instructions::OP_5XY0(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_6XNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg = (opcode & 0x0F00u) >> 8u;
        uint8_t byte = opcode & 0x00FFu;

        chip8_ptr->registers->at(reg) = byte;
    }

    void Instructions::OP_7XNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_8(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        (this->*eight_dispatch_table[(opcode & 0x000Fu)])(chip8_ptr);    // masks last bit and
        // executes
    }

    /**
     * @brief LD Vx, Vy
     *
     * Set Vx = Vy.
     *
     * Stores the value of register Vy in register Vx.
     *
     * @param chip8_ptr
    */
    void Instructions::OP_8XY0(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg_x = (opcode & 0x0F00u) >> 8u;
        uint8_t reg_y = (opcode & 0x00F0u) >> 4u;

        uint8_t value_y = chip8_ptr->registers->at(reg_y);
        chip8_ptr->registers->at(reg_x) = value_y;
    }

    /**
     * @brief OR Vx, Vy
     *
     * Set Vx = Vx OR Vy.
     *
     * Performs a bitwise OR on the values of Vx and Vy, then stores the result in Vx.
     * A bitwise OR compares the corresponding bits from two values, and if either bit is 1,
     * then the same bit in the result is also 1. Otherwise, it is 0.
     *
     * @param chip8_ptr
    */
    void Instructions::OP_8XY1(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg_x = (opcode & 0x0F00u) >> 8u;
        uint8_t reg_y = (opcode & 0x00F0u) >> 4u;

        uint8_t value_x = chip8_ptr->registers->at(reg_x);
        uint8_t value_y = chip8_ptr->registers->at(reg_y);

        uint8_t c = static_cast<uint8_t>(value_x | value_y);    // OR operator
        chip8_ptr->registers->at(reg_x) = c;
    }

    /**
     * @brief AND Vx, Vy
     *
     * Set Vx = Vx AND Vy.
     *
     * Performs a bitwise AND on the values of Vx and Vy, then stores the result in Vx.
     * A bitwise AND compares the corresponding bits from two values, and if both bit is 1,
     * then the same bit in the result is also 1. Otherwise, it is 0.
     *
     * @param chip8_ptr
    */
    void Instructions::OP_8XY2(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg_x = (opcode & 0x0F00u) >> 8u;
        uint8_t reg_y = (opcode & 0x00F0u) >> 4u;

        uint8_t value_x = chip8_ptr->registers->at(reg_x);
        uint8_t value_y = chip8_ptr->registers->at(reg_y);

        uint8_t c = static_cast<uint8_t>(value_x & value_y);
        chip8_ptr->registers->at(reg_x) = c;
    }

    /**
     * @brief XOR Vx, Vy
     *
     * Set Vx = Vx XOR Vy.
     *
     * Performs a bitwise exclusive OR on the values of Vx and Vy, then stores the result in Vx.
     * An exclusive OR compares the corresponding bits from two values, and if the bits are not both the same,
     * then the corresponding bit in the result is set to 1. Otherwise, it is 0.
     *
     * @param chip8_ptr
    */
    void Instructions::OP_8XY3(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg_x = (opcode & 0x0F00u) >> 8u;
        uint8_t reg_y = (opcode & 0x00F0u) >> 4u;

        uint8_t value_x = chip8_ptr->registers->at(reg_x);
        uint8_t value_y = chip8_ptr->registers->at(reg_y);

        uint8_t c = static_cast<uint8_t>(value_x ^ value_y);
        chip8_ptr->registers->at(reg_x);
    }

    /**
     * @brief ADD Vx, Vy
     *
     * Set Vx = Vx + Vy, set VF = carry.
     *
     * The values of Vx and Vy are added together.
     * If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
     * Only the lowest 8 bits of the result are kept, and stored in Vx.
     *
     * @param chip8_ptr
    */
    void Instructions::OP_8XY4(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg_x = (opcode & 0x0F00u) >> 8u;
        uint8_t reg_y = (opcode & 0x00F0u) >> 4u;

        uint8_t value_x = chip8_ptr->registers->at(reg_x);
        uint8_t value_y = chip8_ptr->registers->at(reg_y);

        uint16_t full_sum_value = value_x + value_y;
        chip8_ptr->registers->at(0xF) = 0;
        if (full_sum_value > 255) {
            chip8_ptr->registers->at(0xF) = 1; // VF (carry) = 1
        }
        chip8_ptr->registers->at(reg_x) = (full_sum_value & 0x00FF); // 8 lowest bits
    }

    /**
     * @brief SUB Vx, Vy
     *
     * Set Vx = Vx - Vy, set VF = NOT borrow.
     *
     * If Vx > Vy, then VF is set to 1, otherwise 0. Then Vy is subtracted from Vx, and the results stored in Vx.
     *
     * @param chip8_ptr
     */
    void Instructions::OP_8XY5(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg_x = (opcode & 0x0F00u) >> 8u;
        uint8_t reg_y = (opcode & 0x00F0u) >> 4u;

        uint8_t value_x = chip8_ptr->registers->at(reg_x);
        uint8_t value_y = chip8_ptr->registers->at(reg_y);

        chip8_ptr->registers->at(0xF) = 0;
        if (value_x > value_y) chip8_ptr->registers->at(0xF) = 1;

        uint16_t full_diff = value_x - value_y;
        chip8_ptr->registers->at(reg_x) = (full_diff & 0x00FF); // 8 lowest bits
    }

    /**
     * @brief SHR Vx {, Vy}
     *
     * Set Vx = Vx SHR 1.
     *
     * If the least-significant bit of Vx is 1, then VF is set to 1, otherwise 0.
     * Then Vx is divided by 2.
     *
     * @param chip8_ptr
     */
    void Instructions::OP_8XY6(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg_x = (opcode & 0x0F00u) >> 8u;

        uint8_t value_x = chip8_ptr->registers->at(reg_x);
        uint8_t lsb_x = (value_x & 0x000Fu);
        chip8_ptr->registers->at(0xF) = 0;
        if (lsb_x == 1) chip8_ptr->registers->at(0xF) = 1;

        chip8_ptr->registers->at(reg_x) = (value_x / 2);
    }

    /**
     * @brief SUBN Vx, Vy
     *
     * Set Vx = Vy - Vx, set VF = NOT borrow.
     *
     * If Vy > Vx, then VF is set to 1, otherwise 0.
     * Then Vx is subtracted from Vy, and the results stored in Vx.
     *
     * @param chip8_ptr
     */
    void Instructions::OP_8XY7(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg_x = (opcode & 0x0F00u) >> 8u;
        uint8_t reg_y = (opcode & 0x00F0u) >> 8u;

        uint8_t value_x = chip8_ptr->registers->at(reg_x);
        uint8_t value_y = chip8_ptr->registers->at(reg_y);

        chip8_ptr->registers->at(0xF) = 0;
        if (value_y > value_x) chip8_ptr->registers->at(0xF) = 1;

        uint16_t full_diff = value_y - value_x;
        chip8_ptr->registers->at(reg_x) = (full_diff & 0x00FF); // 8 lowest bits
    }

    /**
     * @brief SHL Vx {, Vy}
     * Set Vx = Vx SHL 1.
     *
     * If the most-significant bit of Vx is 1, then VF is set to 1, otherwise to 0.
     * Then Vx is multiplied by 2.
     *
     * @param chip8_ptr
     */
    void Instructions::OP_8XYE(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg_x = (opcode & 0x0F00u) >> 8u;

        uint8_t value_x = chip8_ptr->registers->at(reg_x);
        uint8_t msb_x = (value_x & 0xF000u);
        chip8_ptr->registers->at(0xF) = 0;
        if (msb_x == 1) chip8_ptr->registers->at(0xF) = 1; // VF = 1

        chip8_ptr->registers->at(reg_x) = (value_x * 2);
    }

    /**
     * @brief SNE Vx, Vy
     * Skip next instruction if Vx != Vy.
     *
     * The values of Vx and Vy are compared, and if they are not equal, the program counter is increased by 2.
     *
     * @param chip8_ptr
     */
    void Instructions::OP_9XY0(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg_x = (opcode & 0x0F00u) >> 8u;
        uint8_t reg_y = (opcode & 0x00F0u) >> 8u;

        uint8_t value_x = chip8_ptr->registers->at(reg_x);
        uint8_t value_y = chip8_ptr->registers->at(reg_y);

        if (value_x != value_y) chip8_ptr->program_ctr += 2;
    }

    /**
     * @brief LD I, addr
     *
     * Set I = nnn.
     *
     * The value of register I is set to nnn.
     *
     * @param chip8_ptr
     */
    void Instructions::OP_ANNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint16_t addr = opcode & 0x0FFFu;
        chip8_ptr->index_reg = addr;
    }

    void Instructions::OP_BNNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_CXNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg = (opcode & 0x0F00u) >> 8;
        uint8_t byte = (opcode & 0x00FFu);

        chip8_ptr->registers->at(reg) = chip8_ptr->get_random_number() & byte;
    }

    void Instructions::OP_DXYN(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint16_t addr = chip8_ptr->index_reg;
        uint8_t* sprite_ptr = chip8_ptr->memory->data() + addr;

        uint8_t bytes = opcode & 0x000Fu;

        uint8_t x = (opcode & 0x0F00u) >> 8u;
        uint8_t y = (opcode & 0x00F0u) >> 4u;

        uint8_t f = 0;

        for (int i = 0; (i < bytes); i++) {  // add artificial upper bound at n bytes
            draw(*sprite_ptr, x + i, y, chip8_ptr);
            sprite_ptr++;
        }

        chip8_ptr->registers->at(0xF) = f;
    }

    void Instructions::OP_E(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        (this->*e_dispatch_table[(opcode & 0x000Fu)])(chip8_ptr);
    }

    void Instructions::OP_EX9E(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_EXA1(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    // F-Ops
    void Instructions::OP_F(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        (this->*f_dispatch_table[(opcode & 0x00FFu)])(chip8_ptr);    // masks last two bits and
        // executes
    }

    /**
     * @brief LD Vx, DT
     *
     * Set Vx = delay timer value.
     *
     * The value of DT is placed into Vx.
     *
     * @param chip8_ptr
     */
    void Instructions::OP_FX07(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg = (opcode & 0x0F00u) >> 8u;
        uint8_t delay_v = chip8_ptr->delay_timer;

        chip8_ptr->registers->at(reg) = delay_v;
    }

    /**
     * @brief LD Vx, K
     *
     * Wait for a key press, store the value of the key in Vx.
     * All execution stops until a key is pressed, then the value of that key is stored in Vx.
     *
     * @param chip8_ptr
     */
    void Instructions::OP_FX0A(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg_x = (opcode & 0x0F00u) >> 8u;
        chip8_ptr->set_waiting_register(reg_x);
        // value of key is stored in Vx at SDL_KEYUP inside Platform.cpp
    }

    void Instructions::OP_FX15(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg = (opcode & 0x0F00u) >> 8u;
        uint8_t v = chip8_ptr->registers->at(reg);

        chip8_ptr->delay_timer = v;
    }

    void Instructions::OP_FX18(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg = (opcode & 0x0F00u) >> 8u;
        uint8_t v = chip8_ptr->registers->at(reg);

        chip8_ptr->sound_timer = v;
    }

    void Instructions::OP_FX1E(std::shared_ptr<Chip8::Chip> chip8_ptr) {
    }

    void Instructions::OP_FX29(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_FX33(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_FX55(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t last_reg = (opcode & 0x0F00u) >> 8u;

        std::array<uint8_t, 16>::iterator reg_begin = chip8_ptr->registers->begin();
        std::array<uint8_t, 16>::iterator reg_end = reg_begin + last_reg;
        std::array<uint8_t, 4096>::iterator mem_start = chip8_ptr->memory->begin() +
        chip8_ptr->index_reg;

        std::copy(reg_begin, reg_end, mem_start);

    }

    void Instructions::OP_FX65(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_NULL(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        std::cout << "Performed null operation" << std::endl;
        return;
    }

    void Instructions::draw(uint8_t sprite_byte, uint8_t x, uint8_t y, std::shared_ptr<Chip8::Chip>
    chip8_ptr) {
        for (int bit = 0; bit < 8; bit++) {
            uint8_t sprite_pixel = sprite_byte & (0b10000000u >> bit); // mask out single bit
            uint8_t* gfx_ptr = &chip8_ptr->gfx->at(x).at(y);

            if (sprite_pixel && *gfx_ptr) {
                chip8_ptr->registers->at(0xF) = 1;
            }

            *(gfx_ptr + bit) ^= sprite_pixel;
        }
    }

    
} // Chip8
