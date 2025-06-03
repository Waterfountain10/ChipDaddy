#include <iostream>
#include <fstream>
#include <format>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "src/hardware/chip.h"
#include <boost/type_index.hpp>
#include <type_traits>

#include "src/Platform.h"

#include "src/gui/gui.h"

template<typename T>
bool __checkType(const T& value) {
    if constexpr (std::is_same_v<T, int>) {
        return true;
    } else {
        return false;
    }
}

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // default values
    std::string rom_path;
    std::ifstream rom_file;
    int ipf;

    try {
        switch (argc) {
            case 3: {
                // CLI mode
                rom_path = argv[1];
                rom_file.open(rom_path, std::ios::in | std::ios::binary | std::ios::ate);  // pointing seeker at end
                ipf = std::stoi(argv[2]);

                if (!rom_file.is_open())
                    throw std::runtime_error("<rom_path> file could not be opened.");
                if (rom_file.tellg() > 4096)
                    throw std::runtime_error("<rom_path> file size is too big for Chip-8 rom");
                if (rom_file.tellg() < 0)
                    throw std::runtime_error("<rom_path> file size is negative");
                if (!__checkType(ipf))
                    throw std::runtime_error("<ipf> must be a digit");
                if (ipf < 1 || ipf > 20)
                    throw std::runtime_error("<ipf> must be a digit between 1 and 20 inclusively");
                break;
            }
            case 1: {
                // gui mode
                rom_path = "placeholder_text"; // REMOVE LATER
                ipf = 10; // placeholder ipf   // REMOVE LATER

                int demo_w = 621;
                int demo_h = 457;

                std::unique_ptr<Chip8::Gui> intro_gui = std::make_unique<Chip8::Gui>
                ("SELECT_YOUR_ROM",621,
                457, true);

                // intro SDL display loop
                SDL_Event e;
                bool intro_quit = false;
                while (!intro_quit) {
                    while (SDL_PollEvent(&e)) {
                        if (e.type == SDL_QUIT)
                            intro_quit = true;
                    }
                }
                break;
            }
            default: {
                throw std::runtime_error("Incorrect number of arguments. Correct template is: ./chip-8-emulator <ROM_file> <ipf>");
            }
        }
        std::cout << "-------------------------------------------------------" << std::endl;
        std::cout << std::format("Running {}",argv[0]) << std::endl;
        std::cout << std::format("---> ROM: {}", rom_path) << std::endl;
        std::cout << std::format("---> ipf: {}", ipf) << std::endl;
        std::cout << "-------------------------------------------------------" << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 0;
    }

    // Prepare the hardware and gui layer -> dependency injection into platform layer
    std::shared_ptr<Chip8::Chip> chip8_hardware = std::make_shared<Chip8::Chip>(); // DONT FORGET TO ADD WEAK_PTRS
    chip8_hardware->init_instr_dispatcher();
    chip8_hardware->init_gfx();

    // Create a game GUI and the platform
    std::shared_ptr<Chip8::Gui> game_gui = std::make_shared<Chip8::Gui>("GAME",1000,1000, false);
    std::unique_ptr<Chip8::Platform> chip8_platform = std::make_unique<Chip8::Platform>(chip8_hardware, game_gui, ipf);

    // Initialize platform layer
    chip8_platform->add_subsystem(SDL_INIT_VIDEO);
    chip8_platform->add_subsystem(SDL_INIT_EVENTS);

    std::cout << ">>> Added SDL Video and Events." << std::endl;
    chip8_platform->add_subsystem(SDL_INIT_AUDIO);


    chip8_platform->init_sdl();
    std::cout << ">>> Initialized SDL Video and Events." << std::endl;

    // Load the Fonts
    chip8_hardware->load_fonts_in_memory();
    std::cout << ">>> Loaded Fonts in Memory!" << std::endl;

    // Load the ROM
    if (chip8_hardware->load_rom(&rom_file) != 0) {
        std::cout << std::format("The file {} did not load properly.\n", rom_path) << std::endl;
        return -1;
    }
    rom_file.close();   // Closes the file after loading

    // START THE GAME
    std::cout << ">>> game started...\n" << std::endl;

    bool running = true;
    while (running && chip8_platform->check_valid()) {
        chip8_platform->run_frame();
    }

    std::cout << "...game ended\n" << std::endl;

    chip8_platform->~Platform(); // End of all SDL subsystems + destruct layer
    return 0;
}