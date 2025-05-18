#include <iostream>
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
    std::string terminal_exec_arg = './chip_8_emulator';
    std::string actual_exec_arg = argv[0];
    int result = terminal_exec_arg.compare(actual_exec_arg);
    if (result == 0) { //
        try {
            if (argc > 1) {
                std::cout << "-------------------------------------------------------" << std::endl;
                std::cout << std::format("Running {} with inputted <ROM_file>:",argv[0]) << std::endl;
                std::cout << argv[1] << std::endl;
                std::cout << "-------------------------------------------------------" << std::endl;
            }
            else if (argc == 3) {
                if (!std::isdigit(*argv[2])) { // not a digit for the ipf parameter -> Error

                }
            }


            else { // Running with ./chip_emulator <rom-file>
                throw std::runtime_error("Incorrect number of arguments. Must have one <ROM_file> only");
            }
        }
        catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

    // Else Running without terminal

    // LOOP1 : while 60 Hz
        // LOOP2 : for int i = 0 -> IPF (dont forget to set up const as argv)



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