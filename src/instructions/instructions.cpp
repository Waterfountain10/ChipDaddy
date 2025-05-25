//
// Created by albert on 5/16/25.
//

#include "instructions.h"

namespace Chip8 {
    // public
    Instructions::Instructions() {

    }

    int Instructions::set_opcode(uint16_t opcode) {

    }

    int Instructions::interpret_opcode(uint16_t opcode) {   // static
        return 0;
    }

    // private
    void Instructions::OP_00E0() {

    }

    // TODO: Add all the remaining instructions
    void OP_00E0();
    void OP_00EE();

    void OP_1NNN();
    void OP_2NNN();
    void OP_3XNN();
    void OP_4XNN();
    void OP_5XY0();
    void OP_6XNN();
    void OP_7XNN();

    void OP_8XY0();
    void OP_8XY1();
    void OP_8XY2();
    void OP_8XY3();
    void OP_8XY4();
    void OP_8XY5();
    void OP_8XY6();
    void OP_8XY7();
    void OP_8XYE();

    void OP_9XY0();

    void OP_ANNN();
    void OP_BNNN();
    void OP_CXNN();
    void OP_DXYN();
    void OP_EX9E();
    void OP_EXA1();

    // F-Ops
    void OP_Fx07();
    void OP_Fx0A();
    void OP_Fx15();
    void OP_Fx18();
    void OP_Fx1E();
    void OP_Fx29();
    void OP_Fx33();
    void OP_Fx55();
    void OP_Fx65();
} // Chip8
