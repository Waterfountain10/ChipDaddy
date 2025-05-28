//
// Created by albert on 5/16/25.
//

#include "instructions.h"

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

    void Instructions::OP_8XY0(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_8XY1(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_8XY2(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_8XY3(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }
    void Instructions::OP_8XY4(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }
    void Instructions::OP_8XY5(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }
    void Instructions::OP_8XY6(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }
    void Instructions::OP_8XY7(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }
    void Instructions::OP_8XYE(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_9XY0(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_ANNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint16_t addr = opcode & 0x0FFFu;
        chip8_ptr->index_reg = addr;
    }

    void Instructions::OP_BNNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_CXNN(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_DXYN(std::shared_ptr<Chip8::Chip> chip8_ptr) {

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

    void Instructions::OP_FX07(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        uint8_t reg = (opcode & 0x0F00u) >> 8u;
        uint8_t v = chip8_ptr->delay_timer;

        chip8_ptr->registers->at(reg) = v;
    }

    void Instructions::OP_FX0A(std::shared_ptr<Chip8::Chip> chip8_ptr) {

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
        
    }

    void Instructions::OP_FX65(std::shared_ptr<Chip8::Chip> chip8_ptr) {

    }

    void Instructions::OP_NULL(std::shared_ptr<Chip8::Chip> chip8_ptr) {
        std::cout << "Performed null operation" << std::endl;
        return;
    }
    
} // Chip8
