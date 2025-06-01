//
// Created by albert on 5/17/25.
//

#ifndef PLATFORM_H
#define PLATFORM_H

#include <chrono>
#include <memory>
#include <vector>
#include <set>
#include <map>
#include <SDL_audio.h>
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
    gui_instance, unsigned ipf);
    ~Platform();
    int init_sdl();

    const uint16_t SAMPLE_RATE = 44100;
    const uint16_t BUFFER_SIZE = 4096;
    int init_sdl_audio();

    int add_subsystem(uint32_t subsystem_code);

    int read_input();

    int add_key_state(SDL_Keysym keysym);
    int remove_key_state(SDL_Keysym keysym);

    static void audio_callback(void *userdata, Uint8 *stream, int len);
    void play_sound();
    void disable_sound();

    int create_window_layer(); // TODO

    bool check_valid();

    void run_frame();

private:
    unsigned ipf_;
    const unsigned cycle_hz = 60;
    std::chrono::microseconds cycle_period;
    std::chrono::microseconds last_cycle_time;

    std::unique_ptr<SDL_AudioSpec> want_audio_spec;
    std::unique_ptr<SDL_AudioSpec> have_audio_spec;

    struct AudioData {
        double phase;           // current phase (in 2π) of the oscillator
        double phase_increment; // 2π·frequency/sample_rate
        double frequency;       // Desired pitch in Hz
        bool tone_on;           // whether to emit tone or silence
        int sample_rate;        // default: 48000
        int amplitude;          // max amplitude for 16-bits
    };

    std::unique_ptr<AudioData> curr_audio_data;
};

} // Chip8

#endif //PLATFORM_H
