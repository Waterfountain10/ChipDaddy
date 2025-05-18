#include <iostream>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include<SDL2/SDL_timer.h>
#include "src/hardware/chip8.h"


using namespace Chip8;

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "Error initializing SDL: " << SDL_GetError() << std::endl;
        return 1;
    }

    // If Running on terminal like: ./chip_8_emulator <rom-file>
    //std::string terminal_exec_arg = "./chip_8_emulator";
    //std::string actual_exec_arg = argv[0];
    //int result = terminal_exec_arg.compare(actual_exec_arg);

    // default values (if not running CLI)
    std::string rom_path;
    int ipf;

    // Example: ./chip-8-emulator <rom_path> <ipf>
    try {
        std::ifstream rom_file;
        switch (argc) {
            case 3: // CLI mode
                rom_path = argv[1];
                rom_file.open(rom_path, std::ios::binary | std::ios::ate);

                if (!std::isdigit(argv[2][0]))
                    throw std::runtime_error("<ipf> must be a digit");
                if (int(argv[2][0]) < 1 || int(argv[2][0] > 20))
                    throw std::runtime_error("<ipf> must be a digit between 1 and 20 inclusively");
                if (!rom_file.is_open())
                    throw std::runtime_error("<rom_path> file could not be opened.");
                if (rom_file.tellg() > 4096)
                    throw std::runtime_error("<rom_path> file size is too big for Chip-8 rom");
                if (rom_file.tellg() < 0)
                    throw std::runtime_error("<rom_path> file size is negative");

                // both arguments passed the edge cases
                ipf = int(argv[2][0]);
                rom_path = argv[1];
                break;

            case 1: // GUI mode
                rom_path = "placeholder_text";
                ipf = 10; // placeholder ipf to be replaced later

                break;

            default:
                throw std::runtime_error("Incorrect number of arguments. Correct template is: ./chip-8-emulator <ROM_file> <ipf>");
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

    SDL_Event event;
    bool running = true;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT)
                running = false;
        }
    }

    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}