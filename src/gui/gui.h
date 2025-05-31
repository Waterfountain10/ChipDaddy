//
// Created by William Lafond on 2025-05-18.
//

#ifndef GUI_H
#define GUI_H
#include <SDL_render.h>
#include <SDL_video.h>
#include <string>

namespace Chip8 {

class Gui {
    public:
        Gui(const std::string name, int width, int height, bool is_demo); // constructor
        ~Gui();
        void render();
        void present_idle();
        bool input_rom_path(std::string rom_path);

    private:
        SDL_Window* win = nullptr;
        SDL_Surface* image_surface;
        SDL_Renderer* ren;
        bool intro;
};

} // Chip8

#endif //gui_h
