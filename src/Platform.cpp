//
// Created by albert on 5/17/25.
//

#include "Platform.h"

#include <algorithm>
#include <format>
#include <iostream>
#include <thread>
#include <memory>
#include <map>
#include <set>

#include <SDL.h>

#include "SDL.h"

namespace Chip8 {
    Platform::Platform(std::shared_ptr<Chip8::Chip> chip8_instance, std::shared_ptr<Gui>
    gui_instance, unsigned ipf) :  // initialize list and kep mappings
    sdl_subsystems_(std::make_unique<std::vector<uint32_t>>()),
    key_mapping(
        std::make_unique<std::map<uint8_t, uint8_t>>(
            std::initializer_list<std::pair<const uint8_t, uint8_t>>{
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
        SDL_Quit(); // Gui has proper destructor ~Gui() and Chip is smart pointers = safe destruction
    }

    int Platform::init_sdl(void)
    {
        for (uint32_t subsystem : *(this->sdl_subsystems_)) {
            if (SDL_InitSubSystem(subsystem) != 0) return -1;
        }

        init_sdl_audio();
        return 0;
    }

    int Platform::init_sdl_audio(void)
    {
        curr_audio_data = std::make_unique<AudioData>();
        curr_audio_data->phase       = 0.0;
        curr_audio_data->frequency   = 440.0;     // default beep frequency (A4)
        curr_audio_data->sample_rate = 48000;     // 48 kHz
        curr_audio_data->amplitude   = 28000;
        curr_audio_data->tone_on     = false;
        curr_audio_data->phase_increment =
            (2.0 * M_PI * curr_audio_data->frequency) / curr_audio_data->sample_rate;

        want_audio_spec = std::make_unique<SDL_AudioSpec>();
        want_audio_spec->freq = curr_audio_data->sample_rate;
        want_audio_spec->format   = AUDIO_S16SYS;       // signed 16-bit
        want_audio_spec->channels = 1;                  // mono
        want_audio_spec->samples  = 1024;               // buffer size
        want_audio_spec->callback = Platform::audio_callback;
        want_audio_spec->userdata = curr_audio_data.get();

        SDL_AudioSpec have;
        SDL_zero(have);
        SDL_AudioDeviceID dev = SDL_OpenAudioDevice(
            nullptr,      // default output device
            0,            // playback (not capture)
            want_audio_spec.get(),        // our requested spec
            &have,        // what SDL actually provides
            0             // do not allow any automatic changes
        );
        this->have_audio_spec = std::make_unique<SDL_AudioSpec>(have);

        if (dev == 0) {
            std::cerr << "SDL_OpenDeviceError" << SDL_GetError();
            SDL_Quit();
            return -1;
        }

        SDL_PauseAudioDevice(dev, 0);
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
        // Printing current key states for debugging
        if (!chip8_->key_states->empty()) {
            for (int v : *chip8_->key_states)            // elements come out sorted, no duplicates
                std::cout << v << ' ';
            std::cout << '\n';
        }
        while (SDL_PollEvent(&(this->curr_key_input_event)))
        {
            // read poll event only once
            switch (this->curr_key_input_event.type) {
            case SDL_KEYDOWN:
                // Turn the Key on
                if (this->curr_key_input_event.key.keysym.sym == SDLK_ESCAPE) {
                    SDL_Quit();
                    should_quit = true;
                }
                if (is_valid_key(this->curr_key_input_event.key.keysym)) {
                    this->add_key_state(this->curr_key_input_event.key.keysym);
                }
                break;

            case SDL_KEYUP:
                // If key is off then take the key off
                if (is_valid_key(curr_key_input_event.key.keysym)) {
                    if (chip8_->waiting_reg != 0xFF) {
                        chip8_->complete_key_wait(
                            key_mapping->find(curr_key_input_event.key.keysym.sym)->second
                        );
                    }; // avoid seg fault
                    this->remove_key_state(curr_key_input_event.key.keysym);
                }
                break;

            case SDL_QUIT:
                // Prompts SDL to close the window and program
                SDL_Quit();
                // Rest is dynamically handled
                should_quit = true;
                break;

            default:
                // Do nothing
                break;
            }
        }
        return 0;
    }

    bool Platform::is_valid_key(SDL_Keysym keysym) {
        std::map<uint8_t, uint8_t>::iterator mapping = key_mapping->find(keysym.sym);
        return !(mapping == key_mapping->end());
    }

    int Platform::add_key_state(SDL_Keysym keysym) {
        // add to key_states
        uint8_t key = this->key_mapping->at(keysym.sym);
        chip8_->add_key_state(key);
        return 0;
    }

    int Platform::remove_key_state(SDL_Keysym keysym) {
        // remove from key_states
        uint8_t key = this->key_mapping->at(keysym.sym);
        return chip8_->remove_key_state(key);
    }

    /**
     * @brief SDL audio callback that generates a tone or silence based on AudioData state.
     *
     * This function is called by SDL when the audio device needs more samples. It casts
     * the provided userdata to an AudioData pointer, then fills the output buffer with
     * either a sine wave (if audio->sound_on is true) or zeros (silence).
     *
     * @param userdata Pointer to an AudioData instance containing the oscillator state.
     * @param stream   Pointer to the audio buffer that SDL expects to be filled (Uint8*).
     * @param len      Length of the audio buffer in bytes.
     */
    void Platform::audio_callback(void *userdata, Uint8 *stream, int len) {
        Platform::AudioData* audio_data = static_cast<Platform::AudioData*>(userdata);
        Sint16* buf = reinterpret_cast<Sint16*>(stream);    // convert the stream to 16-bit samples
        int samples = len / sizeof(Sint16);  // number of 16-bit samples

        for (int i = 0; i < samples; i++) {
            if (audio_data->tone_on) {
                // Generate a sine wave at the current phase
                std::cout << buf[i] << std::endl;
                double value = std::sin(audio_data->phase);
                buf[i] = static_cast<Sint16>(audio_data->amplitude * value);

                // Increment phase and wrap around at 2π to keep it in range
                audio_data->phase += audio_data->phase_increment;
                if (audio_data->phase >= 2.0 * M_PI) {
                    audio_data->phase -= 2.0 * M_PI;
                }
                std::cout << audio_data->phase << std::endl;
            } else {
                // Output silence when the sound is off
                buf[i] = 0;
            }
        }
    }

    void Platform::play_sound() {
        curr_audio_data->tone_on = true;
    }

    void Platform::disable_sound() {
        curr_audio_data->tone_on = false;
    }

    bool Platform::check_valid() {
        // true if rom loaded and no quitting key has been pressed
        return chip8_->get_rom_loaded() && !should_quit;
    }

    void Platform::run_frame() {
        std::chrono::time_point frame_start_time = std::chrono::steady_clock::now();

        // Run instructions per frame as specified;
        for (int i = 0; i < ipf_; ++i) {
            read_input();
            if (!chip8_->is_waiting_for_key()) { // skip cycle if waiting for a key
                chip8_->cycle();
            }

        }

        // DRAW PIXEL in gui as a rectangle (or square to be exact)
        gui_->clear();
            for (int x = 0; x < 64; x++) {
                for (int y = 0; y < 32; y++) {
                    if (chip8_->gfx->at(x).at(y)) // if y exists then, pixel is valid
                        gui_->draw_pixel(x,y,true);
                }
            }
        gui_->present_idle();

        chip8_->decrement_timers();

        // Plays sound based on condition
        if (chip8_->sound_timer > 0 && !curr_audio_data->tone_on) {
            play_sound();
        }
        else if (chip8_->sound_timer == 0 && curr_audio_data->tone_on) {
            disable_sound();
        }

        // Compute remaining time
        std::chrono::time_point<std::chrono::steady_clock> frame_end_time = std::chrono::steady_clock::now();
        std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::milliseconds>
        (frame_end_time - frame_start_time);
        std::chrono::microseconds time_to_wait = cycle_period - elapsed;

        // Sleep until time is up
        std::this_thread::sleep_for(time_to_wait);
    }

} // Chip8