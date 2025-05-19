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
GUI::GUI(std::string name, int width, int height, bool is_demo) {

    // initialize a window display
    SDL_Window* gui_display = SDL_CreateWindow(name.c_str(),
                                    SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED,
                                    width,height,
                                    SDL_WINDOW_SHOWN);

    // unopened GUI exception handled in main.cpp
    if (gui_display == NULL)
        throw std::runtime_error("GUI could not be opened!");

    // load window surface and image surface
    SDL_Surface* gui_display_surface = SDL_GetWindowSurface(gui_display);
    SDL_Surface* image_surface;
    if (is_demo) {
        std::cout << "its here" << std::endl;
        image_surface = SDL_LoadBMP("../src/image/demo_gui.bmp");
        if (image_surface == NULL) {
            std::cout << "FUCK ME ITS NOT WORKING THE IMAGE" << std::endl;
        }
    }
    else { // actual game display (not the first GUI)
        // TODO : Implement the GAME DISPLAY
    }

    // paste that image_surface on window_surface
    SDL_BlitSurface(image_surface, NULL, gui_display_surface,NULL);
    SDL_UpdateWindowSurface( gui_display );

    // SDL display loop
    SDL_Event e;
    bool quit = false;

    while (quit == false) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
        }
    }
}
// DESTRUCTOR
GUI::~GUI() {
    if (win)
        SDL_DestroyWindow(win);
    SDL_Quit();
}
}
