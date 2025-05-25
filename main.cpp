#include <iostream>
#include <fstream>
#include <format>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>
#include "src/hardware/chip8.h"
#include <boost/type_index.hpp>
#include <type_traits>

#include "src/Platform.h"

#include "src/GUI/GUI.h"

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
    int ipf;

    try {
        std::ifstream rom_file;
        switch (argc) {
            case 3: {
                // CLI mode
                rom_path = argv[1];
                rom_file.open(rom_path, std::ios::binary | std::ios::ate);
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
                // GUI mode
                rom_path = "placeholder_text"; // REMOVE LATER
                ipf = 10; // placeholder ipf   // REMOVE LATER

                int demo_w = 621;
                int demo_h = 457;

                std::unique_ptr<GUI::GUI> intro_gui = std::make_unique<GUI::GUI>("SELECT_YOUR_ROM",621,457, true);

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

    std::shared_ptr<Chip8::Chip8> chip8_hardware = std::make_shared<Chip8::Chip8>(); // DONT FORGET TO ADD WEAK_PTRS
    std::shared_ptr<Chip8::Platform> chip8_platform = std::make_shared<Chip8::Platform>(chip8_hardware);

    // Load the fonts in memory
    chip8_hardware->load_fonts_in_memory();

    bool running = true;
    while (running) {
        chip8_platform->read_input();
        break; // for now since read_input() is not implemented yet
    }


    // START THE GAME
    std::cout << "game started..." << std::endl;
    std::unique_ptr<GUI::GUI> game_gui = std::make_unique<GUI::GUI>("GAME",1000,1000, false);
    SDL_Event e;
    bool game_quit = false;
    while (!game_quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                game_quit = true;
        }
    }
    std::cout << "...game ended" << std::endl;

    SDL_Quit(); // End of all SDL subsystems
    return 0;
}