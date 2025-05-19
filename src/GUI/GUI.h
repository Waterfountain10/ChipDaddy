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
        SDL_Window* win;
        GUI(std::string name, int width, int height, bool is_demo); // constructor
        ~GUI();

        bool input_rom_path(std::string rom_path);

    private:
        // nothing for now
    };
}

#endif //GUI_H
