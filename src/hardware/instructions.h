//
// Created by albert on 5/16/25.
//

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H
#include <array>
#include <cstdint>
#include <iostream>

#include "chip.h"

namespace Chip8 {

    class Chip; // avoid circular declarations

    class Instructions {
    public:
        static constexpr std::size_t NUM_OPS = 35;
        uint16_t opcode;

        explicit Instructions(std::shared_ptr<Chip> chip8_instance);    // constructor
        ~Instructions() = default;

        int interpret_opcode(uint16_t opcode);   // Computed goto table

        using Handler = void (Instructions::*)(std::shared_ptr<Chip8::Chip>);   // define function
        // ptr type

    private:
        static constexpr std::size_t DISPATCH_SIZE = 16;

        std::weak_ptr<Chip8::Chip> chip8_;    // weak_ptr to break circular dependency
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
        void OP_0(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_0NNN(std::shared_ptr<Chip8::Chip> chip8_ptr); // Call
        void OP_00E0(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_00EE(std::shared_ptr<Chip8::Chip> chip8_ptr);

        void OP_1NNN(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_2NNN(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_3XNN(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_4XNN(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_5XY0(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_6XNN(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_7XNN(std::shared_ptr<Chip8::Chip> chip8_ptr);

        // 8-Ops
        void OP_8(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_8XY0(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_8XY1(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_8XY2(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_8XY3(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_8XY4(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_8XY5(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_8XY6(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_8XY7(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_8XYE(std::shared_ptr<Chip8::Chip> chip8_ptr);

        void OP_9XY0(std::shared_ptr<Chip8::Chip> chip8_ptr);

        void OP_ANNN(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_BNNN(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_CXNN(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_DXYN(std::shared_ptr<Chip8::Chip> chip8_ptr);

        // E-Ops
        void OP_E(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_EX9E(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_EXA1(std::shared_ptr<Chip8::Chip> chip8_ptr);

        // F-Ops
        void OP_F(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_FX07(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_FX0A(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_FX15(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_FX18(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_FX1E(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_FX29(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_FX33(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_FX55(std::shared_ptr<Chip8::Chip> chip8_ptr);
        void OP_FX65(std::shared_ptr<Chip8::Chip> chip8_ptr);

        void OP_NULL(std::shared_ptr<Chip8::Chip> chip8_ptr);
    };
}

#endif //INSTRUCTIONS_H
