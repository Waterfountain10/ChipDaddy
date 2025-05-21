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

using namespace Chip8;

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

                GUI::GUI("SELECT_YOUR_ROM",621,457, true);
                // this waits until we close the window
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

    SDL_Window* win = SDL_CreateWindow("GAME",
                                       SDL_WINDOWPOS_CENTERED,
                                       SDL_WINDOWPOS_CENTERED,
                                       1000, 1000,
                                       SDL_WINDOW_SHOWN);
    if (!win) {
        std::cerr << "Failed to create window: " << SDL_GetError() << std::endl;
        return 1;
    }

    std::shared_ptr<Platform> chip8_platform = std::make_shared<Platform>();

    bool running = true;

    while (running) {
        chip8_platform->read_input();
        // std::cout << "Reading input\n" << std::endl;
    }

    SDL_DestroyWindow(win);
    SDL_Quit();

    // START THE GAME
    GUI::GUI("GAME",1000,1000, false);
    return 0;
}