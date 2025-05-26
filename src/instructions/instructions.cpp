//
// Created by albert on 5/16/25.
//

#include "instructions.h"

namespace Chip8 {
    // public
    Instructions::Instructions() {  // constructor
        // logic to add all the opcode labels to the dispatch table
    }

    int Instructions::interpret_opcode(uint16_t p_opcode) {   // static
        opcode = p_opcode;
        (this->*dispatch_table[(opcode & 0xF000u) >> 12u])();   // executes the specific instruction
        return 0;
    }

    // private
    void Instructions::init_dispatch_table() {
        zero_dispatch_table = std::array<Handler, ZERO_OPS>{};
        zero_dispatch_table[0x0] = &Instructions::OP_00E0;
        zero_dispatch_table[0xE] = &Instructions::OP_00EE;

        eight_dispatch_table = std::array<Handler, EIGHT_OPS>{};
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
        e_dispatch_table[0x1] = &Instructions::OP_EXA1;
        e_dispatch_table[0xE] = &Instructions::OP_EX9E;

        f_dispatch_table = std::array<Handler, F_OPS>{};
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
    void Instructions::OP_0() {
        (this->*zero_dispatch_table[(opcode & 0x000Fu)])();     // masks last bit and executes
    }
    void Instructions::OP_00E0() {

    };
    void Instructions::OP_00EE() {

    };

    void Instructions::OP_1NNN() {

    };
    void Instructions::OP_2NNN() {

    };
    void Instructions::OP_3XNN() {

    };
    void Instructions::OP_4XNN() {

    };
    void Instructions::OP_5XY0() {

    };
    void Instructions::OP_6XNN() {

    };
    void Instructions::OP_7XNN() {

    };

    void Instructions::OP_8() {
        (this->*eight_dispatch_table[(opcode & 0x000Fu)])();    // masks last bit and executes
    }
    void Instructions::OP_8XY0() {

    };
    void Instructions::OP_8XY1() {

    };
    void Instructions::OP_8XY2() {

    };
    void Instructions::OP_8XY3() {

    };
    void Instructions::OP_8XY4() {

    };
    void Instructions::OP_8XY5() {

    };
    void Instructions::OP_8XY6() {

    };
    void Instructions::OP_8XY7() {

    };
    void Instructions::OP_8XYE() {

    };

    void Instructions::OP_9XY0() {

    };

    void Instructions::OP_ANNN() {

    };
    void Instructions::OP_BNNN() {

    };
    void Instructions::OP_CXNN() {

    };
    void Instructions::OP_DXYN() {

    };

    void Instructions::OP_E() {
        (this->*e_dispatch_table[(opcode & 0x000Fu)])();
    }

    void Instructions::OP_EX9E() {

    };

    void Instructions::OP_EXA1() {

    };

    // F-Ops
    void Instructions::OP_F() {
        (this->*f_dispatch_table[(opcode & 0x00FFu)])();    // masks last two bits and executes
    }

    void Instructions::OP_FX07() {

    };
    void Instructions::OP_FX0A() {

    };
    void Instructions::OP_FX15() {

    };
    void Instructions::OP_FX18() {

    };
    void Instructions::OP_FX1E() {

    };
    void Instructions::OP_FX29() {

    };
    void Instructions::OP_FX33() {

    };
    void Instructions::OP_FX55() {

    };
    void Instructions::OP_FX65() {

    };
} // Chip8
