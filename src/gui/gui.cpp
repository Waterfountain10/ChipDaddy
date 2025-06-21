//
// Created by William Lafond on 2025-05-18.
//

#include "gui.h"

#include <iostream>
#include <type_traits>

#include <SDL_events.h>
#include <SDL_video.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_timer.h>

namespace Chip8 {
    // CONSTRUCTOR
    Gui::Gui(const std::string name, int w, int h, bool is_intro) {
        width = w;
        height = h;

        // initialize a window display
        this->win = SDL_CreateWindow(name.c_str(),
                                        SDL_WINDOWPOS_CENTERED,
                                        SDL_WINDOWPOS_CENTERED,
                                        width,height,
                                        SDL_WINDOW_RESIZABLE);

        // unopened gui exception handled in main.cpp
        if (!win)
            throw std::runtime_error("gui could not be opened!");

        //SDL_Surface* gui_display_surface = SDL_GetWindowSurface(win); // SDL allocates this pointer so no need to free
        this->ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
        if (!ren)
            throw std::runtime_error("SDL_CreateRenderer failed");

        SDL_RenderSetLogicalSize(this->ren, width, height);

        // intro display (for rom selection screen)
        if (is_intro) {
            std::cout << "Running the gui select screen : Select your ROM" << std::endl;
            SDL_SetRenderDrawColor(ren, 50, 50, 200, 255);
        }
        else {
            SDL_SetRenderDrawColor(ren, 10, 10, 10, 255);
        }

        SDL_Surface* screen_surface = SDL_CreateRGBSurfaceWithFormat(
                0,
                width,
                height,
                1,
                SDL_PIXELFORMAT_INDEX8
            );

        if (!screen_surface) {
            SDL_Log("Surface has no palette?!\n");
            SDL_FreeSurface(screen_surface);
            return;
        }

        SDL_Palette* palette = screen_surface->format->palette;

        if (!palette) {
            SDL_Log("Surface has no palette?!\n");
            SDL_FreeSurface(screen_surface);
            return;
        }

        SDL_SetPaletteColors(palette, init_colors(), 0, 2);
        screen_texture = SDL_CreateTextureFromSurface(ren, screen_surface);

        screen_rect = std::make_unique<SDL_Rect>( SDL_Rect{ 0, 0, width, height } );

        SDL_FreeSurface(screen_surface);
        // one time clear + present so you see something right away
        SDL_RenderClear(ren);
        SDL_RenderCopy(ren, screen_texture, NULL, screen_rect.get());
        SDL_RenderPresent(ren);
    }

    SDL_Color* Gui::init_colors() {
        colors = static_cast<SDL_Color*>(malloc(sizeof(SDL_Color) * 2));

        SDL_Color white = SDL_Color{ 0, 0, 0, 255 };
        SDL_Color black = SDL_Color{ 255, 255, 255, 255 };
        colors[0] = black;
        colors[1] = white;

        return colors;
    }

    // DESTRUCTOR
    Gui::~Gui() {
        if (screen_texture) SDL_DestroyTexture(screen_texture);
        if (ren) SDL_DestroyRenderer(ren);
        if (win) SDL_DestroyWindow(win);
        if (colors) free(colors);
        ren = nullptr; win = nullptr; screen_texture = nullptr; screen_rect = nullptr;
    }

    void Gui::clear() {
        SDL_SetRenderDrawColor(ren, 20, 20, 20, 255);
        SDL_RenderClear(ren);
    }

    void Gui::present_idle() {
        SDL_RenderPresent(ren);
    }

    /**
     * Draws a single CHIP-8 “pixel” as a filled rectangle on the screen.
     *
     *
     * @param col    The horizontal coordinate of the pixel (0–63).
     * @param row    The vertical coordinate of the pixel (0–31).
     * @param scale  The size, in screen‐pixels, of one CHIP-8 pixel.
     * @param on     If true, draw the pixel (white); if false, do nothing (pixel remains off).
    */
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

    int Gui::update_texture(uint8_t* gfx_ptr) {
        SDL_UpdateTexture(screen_texture, nullptr, gfx_ptr, width * sizeof(uint8_t));
        return SDL_RenderCopy(ren, screen_texture, NULL, screen_rect.get());
    }

}
