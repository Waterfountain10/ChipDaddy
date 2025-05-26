//
// Created by albert on 5/16/25.
//

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <array>
#include <cstdint>

namespace Chip8 {
    class Instructions {
    public:
        static constexpr std::size_t NUM_OPS = 35;
        uint16_t opcode;

        explicit Instructions();    // constructor
        ~Instructions() = default;

        int interpret_opcode(uint16_t opcode);   // Computed goto table

        using Handler = void (Instructions::*)();   // define function ptr type

    private:
        static constexpr std::size_t DISPATCH_SIZE = 16;
        std::array<Handler, DISPATCH_SIZE> dispatch_table;  // func_ptr[35]

        static constexpr std::size_t ZERO_OPS = 0x10; // 2
        static constexpr std::size_t EIGHT_OPS = 0x10; // 9
        static constexpr std::size_t E_OPS = 0x10; // 2
        static constexpr std::size_t F_OPS = 0x100; // 9

        std::array<Handler, ZERO_OPS> zero_dispatch_table;
        std::array<Handler, EIGHT_OPS> eight_dispatch_table;
        std::array<Handler, E_OPS> e_dispatch_table;
        std::array<Handler, F_OPS> f_dispatch_table;

        void init_dispatch_table();

        // 0-Ops
        void OP_0();
        void OP_0NNN(); // Call
        void OP_00E0();
        void OP_00EE();

        void OP_1NNN();
        void OP_2NNN();
        void OP_3XNN();
        void OP_4XNN();
        void OP_5XY0();
        void OP_6XNN();
        void OP_7XNN();

        // 8-Ops
        void OP_8();
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

        // E-Ops
        void OP_E();
        void OP_EX9E();
        void OP_EXA1();

        // F-Ops
        void OP_F();
        void OP_FX07();
        void OP_FX0A();
        void OP_FX15();
        void OP_FX18();
        void OP_FX1E();
        void OP_FX29();
        void OP_FX33();
        void OP_FX55();
        void OP_FX65();
    };
}

#endif //INSTRUCTIONS_H
