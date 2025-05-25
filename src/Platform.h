//
// Created by albert on 5/17/25.
//

#ifndef PLATFORM_H
#define PLATFORM_H

#include <memory>
#include <vector>
#include <set>
#include <map>
#include <SDL_events.h>

#include "hardware/chip8.h"

namespace Chip8 {

class Platform {
public:
    SDL_Event curr_key_input_event;
    const std::shared_ptr<std::vector<uint32_t>> sdl_subsystems_ ;    // smart_ptr to vector of subsys

    const std::shared_ptr<std::set<uint8_t>> key_states;            // state of keys
    const uint8_t key_input_range = 15;                             // 0 - 15 or 0x0 to 0xF
    const std::shared_ptr<std::map<uint8_t, uint8_t>> key_mapping;

    const std::shared_ptr<Chip8> chip8_;                      // actual hardware

    explicit Platform(std::shared_ptr<Chip8> chip8_instance);         // constructor
    int init_sdl();
    int add_subsystem(uint32_t subsystem_code);
    int read_input();
    int add_key_state(SDL_Keysym keysym);
    int remove_key_state(SDL_Keysym keysym);
    // ~Platform();
};

} // Chip8

#endif //PLATFORM_H
