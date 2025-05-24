//
// Created by William Lafond on 2025-05-18.
//

#ifndef GUI_H
#define GUI_H
#include <SDL_video.h>
#include <string>

namespace GUI {

class GUI {
    public:
        GUI(std::string name, int width, int height, bool is_demo); // constructor
        ~GUI();

        bool input_rom_path(std::string rom_path);

    private:
        SDL_Window* win = nullptr;
        SDL_Surface* image_surface;
};
}

#endif //GUI_H
