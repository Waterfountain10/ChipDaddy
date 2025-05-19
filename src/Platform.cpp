//
// Created by albert on 5/17/25.
//

#include "Platform.h"

#include <algorithm>
#include <memory>
#include <map>
#include <set>
#include <SDL.h>
#include "SDL.h"

namespace Chip8 {
    Platform::Platform(void) :
    sdl_subsystems(std::make_shared<std::vector<uint32_t>>()),
    key_states(std::make_shared<std::set<uint8_t>>()),
    key_mapping(std::make_shared<std::map<uint8_t, uint8_t>>()) // initialize list
    {
        initialize_sdl();
    }

    int Platform::initialize_sdl(void)
    {
        for (uint32_t subsystem : *(this->sdl_subsystems)) {
            if (SDL_InitSubSystem(subsystem) != 0) return -1;
        }
        return 0;
    }

    int Platform::add_subsystem(uint32_t subsystem_code)
    {
        std::shared_ptr<std::vector<uint32_t>> subsystems = this->sdl_subsystems;
        if (std::find(
            subsystems->begin(),
            subsystems->end(),
            subsystem_code
            ) != subsystems->end()) {    // If code is not already in subsystems
            subsystems->push_back(subsystem_code);
            return 0;
        }
        return 1;
    }

    int Platform::read_input()  // main key reading loop call method
    {
        while (SDL_PollEvent(&(this->curr_key_input_event))) { // read poll event only once
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

} // Chip8