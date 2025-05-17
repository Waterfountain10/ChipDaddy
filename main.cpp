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