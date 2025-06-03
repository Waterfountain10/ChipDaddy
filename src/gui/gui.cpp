//
// Created by William Lafond on 2025-05-18.
//

#include "gui.h"

#include <iostream>
#include <SDL_events.h>
#include <SDL_video.h>
#include <type_traits>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

namespace Chip8 {

// CONSTRUCTOR
Gui::Gui(const std::string name, int width, int height, bool is_intro) {

    // initialize a window display
    this->win = SDL_CreateWindow(name.c_str(),
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    width,height,
                                    0);

    // unopened gui exception handled in main.cpp
    if (win == nullptr)
        throw std::runtime_error("gui could not be opened!");

    //SDL_Surface* gui_display_surface = SDL_GetWindowSurface(win); // SDL allocates this pointer so no need to free
    this->ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    if (!ren)
        throw std::runtime_error("SDL_CreateRenderer failed");

    // intro display (for rom selection screen)
    if (is_intro) {
        std::cout << "Running the gui select screen : Select your ROM" << std::endl;
        SDL_SetRenderDrawColor(ren, 50, 50, 200, 255);
    }
    else {
        SDL_SetRenderDrawColor(ren, 10, 10, 10, 255);
    }
    // one time clear + present so you see something right away
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
}


// DESTRUCTOR
Gui::~Gui() {
    if (ren) SDL_DestroyRenderer(ren);
    if (win) SDL_DestroyWindow(win);
    ren = nullptr; win = nullptr;
}

void Gui::clear() {
    SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
    SDL_RenderClear(ren);
}

void Gui::present_idle() {
    SDL_RenderPresent(ren);
}

void Gui::draw_pixel(int col, int row, int scale, bool on)
{
    if (!on) return;
    SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

    SDL_Rect r;
    r.x = col * scale;
    r.y = row * scale;
    r.w = scale;
    r.h = scale;
    SDL_RenderFillRect(ren, &r);
}
}
