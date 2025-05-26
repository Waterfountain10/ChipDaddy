//
// Created by William Lafond on 2025-05-18.
//

#include "GUI.h"

#include <iostream>
#include <SDL_events.h>
#include <SDL_video.h>
#include <type_traits>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

namespace GUI {

// CONSTRUCTOR
GUI::GUI(std::string name, int width, int height, bool is_intro) {

    // initialize a window display
    this->win = SDL_CreateWindow(name.c_str(),
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    width,height,
                                    SDL_WINDOW_SHOWN);

    // unopened GUI exception handled in main.cpp
    if (win == nullptr)
        throw std::runtime_error("GUI could not be opened!");

    SDL_Surface* gui_display_surface = SDL_GetWindowSurface(win); // SDL allocates this pointer so no need to free
    this->image_surface = nullptr;

    // intro display (for rom selection screen)
    if (is_intro) {
        std::cout << "Running the GUI select screen : Select your ROM" << std::endl;
        this->image_surface = SDL_LoadBMP("../src/image/demo_gui.bmp");
        if (image_surface == NULL) {
            std::cout << "The image is not working, fuck." << std::endl;
        }
    }
    else { // actual game display
        // TODO : Implement the GAME DISPLAY
    }

    // paste that image_surface on window_surface
    SDL_BlitSurface(image_surface, NULL, gui_display_surface,NULL);
    SDL_UpdateWindowSurface(win);

}

// DESTRUCTOR
GUI::~GUI() {
    if (image_surface) {
        SDL_FreeSurface(image_surface);
        image_surface = nullptr;
    }
    if (win) {
        SDL_DestroyWindow(win);
        win = nullptr;
    }
}
}
