//
// Created by albert on 5/17/25.
//

#include "Platform.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <memory>
#include <map>
#include <set>
#include <SDL.h>
#include <thread>

#include "SDL.h"

namespace Chip8 {
    Platform::Platform(std::shared_ptr<Chip8::Chip> chip8_instance, std::shared_ptr<Gui>
    gui_instance, unsigned ipf) :  // initialize list and kep mappings
    sdl_subsystems_(std::make_unique<std::vector<uint32_t>>()),
    key_states(std::make_unique<std::set<uint8_t>>()),
    key_mapping(
        std::make_unique<std::map<uint8_t, uint8_t>>(
            std::initializer_list<std::pair<const uint8_t,uint8_t>>{
                {  '1', 0x1 }, {  '2', 0x2 }, {  '3', 0x3 }, {  '4', 0xC },
                { 'q', 0x4 }, { 'w', 0x5 }, { 'e', 0x6 }, { 'r', 0xD },
                { 'a', 0x7 }, { 's', 0x8 }, { 'd', 0x9 }, { 'f', 0xE },
                { 'z', 0xA }, { 'x', 0x0 }, { 'c', 0xB }, { 'v', 0xF }
            }
        )
    ),
    chip8_{ chip8_instance },
    gui_ { gui_instance }
    {
        if (!chip8_) {
            std::cerr << "Platform: Invalid instantiation of Platform Layer\n" << std::endl;
            return;
        }
        if (ipf < 0) {
            std::cerr << "Platform: Invalid IPF (Instructions Per Frame) value\n" << std::endl;
            return;
        }
        chip8_->init_instr_dispatcher();
        ipf_ = ipf;
        cycle_period = std::chrono::microseconds{1'000'000 / cycle_hz};     // calc cycle time
    }

    Platform::~Platform() {
        for (uint32_t subsystem : *(this->sdl_subsystems_)) {
            SDL_QuitSubSystem(subsystem);
        }
        SDL_Quit();
        // TODO: Call any cleanup function from the hardware and gui layer
    }

    int Platform::init_sdl(void)
    {
        for (uint32_t subsystem : *(this->sdl_subsystems_)) {
            if (SDL_InitSubSystem(subsystem) != 0) return -1;
        }
        return 0;
    }

    int Platform::add_subsystem(uint32_t subsystem_code)
    {
        if (std::find(
            sdl_subsystems_->begin(),
            sdl_subsystems_->end(),
            subsystem_code
            ) != sdl_subsystems_->end()) {    // If code is not already in subsystems
            sdl_subsystems_->push_back(subsystem_code);
            return 0;
        }
        return 1;
    }

    int Platform::read_input()  // main key reading loop call method
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                should_quit = true;
                return -1;
            }
            // Printing current key states for debugging
            if (!this->key_states->empty()) {
                for (int v : *this->key_states)            // elements come out sorted, no duplicates
                    std::cout << v << ' ';
                std::cout << '\n';
            }
            while (SDL_PollEvent(&(this->curr_key_input_event)))
            {
                // read poll event only once
                switch (this->curr_key_input_event.type) {
                case SDL_KEYDOWN:
                    // Turn the Key on
                    this->add_key_state(this->curr_key_input_event.key.keysym);
                    break;

                case SDL_KEYUP:
                    // If key is off then take the key off
                    this->remove_key_state(this->curr_key_input_event.key.keysym);
                    break;

                case SDL_QUIT:
                    // Prompts SDL to close the window and program
                    SDL_Quit();
                    // Rest is dynamically handled
                    break;

                default:
                    // Do nothing
                    return 1;
                    break;
                }
            }
        }
        return 0;
    }

    int Platform::add_key_state(SDL_Keysym keysym) {
        // add to key_states
        uint8_t key = this->key_mapping->at(keysym.sym);    // TODO: Add validation
        key_states->insert(key);
        return 0;
    }

    int Platform::remove_key_state(SDL_Keysym keysym) {
        // remove from key_states
        uint8_t key = this->key_mapping->at(keysym.sym);    // TODO: Add validation
        return key_states->erase(key);
    }

    bool Platform::check_valid() {
    //     if (!chip8_->get_rom_loaded()) return false;
    //
    //     return true;
        return chip8_->get_rom_loaded() && !should_quit;
    }

    void Platform::run_frame() {
        std::chrono::time_point frame_start_time = std::chrono::steady_clock::now();
        int cycles_to_run = ipf_;

        // Run instructions per frame as specified;
        for (int i = 0; i < ipf_; ++i) {
            read_input();
            chip8_->cycle();
        }
        // TODO: Update the platform screen after every frame ends
        // TODO: Plays sound using SDL if sound_timer > 0
        chip8_->decrement_timers();

        // Compute remaining time
        std::chrono::time_point<std::chrono::steady_clock> frame_end_time = std::chrono::steady_clock::now();
        std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
        (frame_end_time - frame_start_time);
        std::chrono::microseconds time_to_wait = cycle_period - elapsed;

        gui_->render();

        gui_->present_idle();
        // Sleep until time is up
        if (time_to_wait > std::chrono::microseconds::zero()) {
            std::this_thread::sleep_for(time_to_wait);
        }
    }

} // Chip8