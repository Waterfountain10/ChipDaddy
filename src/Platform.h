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

#include "gui/gui.h"
#include "hardware/chip.h"

namespace Chip8 {

class Platform {
public:
    SDL_Event curr_key_input_event;
    const std::unique_ptr<std::vector<uint32_t>> sdl_subsystems_; // smart_ptr to vector of subsys

    const std::unique_ptr<std::set<uint8_t>> key_states; // state of keys
    const uint8_t key_input_range = 15; // 0 - 15 or 0x0 to 0xF
    const std::unique_ptr<std::map<uint8_t, uint8_t>> key_mapping;

    const std::shared_ptr<Chip> chip8_; // actual hardware
    const std::shared_ptr<Gui> gui_; // gui layer

    explicit Platform(std::shared_ptr<Chip> chip8_instance, std::shared_ptr<Gui>
    gui_instance);
    ~Platform() = default;
    int init_sdl();
    int add_subsystem(uint32_t subsystem_code);
    int read_input();
    int add_key_state(SDL_Keysym keysym);
    int remove_key_state(SDL_Keysym keysym);

    int create_window_layer(); // TODO

    bool check_valid();
};

} // Chip8

#endif //PLATFORM_H
