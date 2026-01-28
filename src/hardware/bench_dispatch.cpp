//
// Created by William Kiem Lafond on 2026-01-28.
//

#include "bench_dispatch.h"
#include <chrono>
#include <iostream>
#include <memory>

#include "chip.h"


int main() {
    auto chip = std::make_shared<Chip8::Chip>();
    chip->init_instr_dispatcher();

    // Pick a very simple opcode: 6XNN (LD Vx, byte)
    const uint16_t opcode = 0x6001; // V0 = 1

    constexpr std::size_t WARMUP = 1'000'000;
    constexpr std::size_t ITERS  = 50'000'000;

    // Warmup
    for (std::size_t i = 0; i < WARMUP; ++i) {
        chip->instr_dispatcher->interpret(opcode);
    }

    auto start = std::chrono::steady_clock::now();

    for (std::size_t i = 0; i < ITERS; ++i) {
        chip->instr_dispatcher->interpret(opcode);
    }

    auto end = std::chrono::steady_clock::now();

    auto ns = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

    double per_op = static_cast<double>(ns) / ITERS;

    std::cout << "Total time (ns): " << ns << "\n";
    std::cout << "ns / instruction: " << per_op << "\n";

    return 0;
}
