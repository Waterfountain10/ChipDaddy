cmake_minimum_required(VERSION 3.31)
project(chip_8_emulator VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(chip_8_emulator main.cpp
        src/hardware/chip8.h
        src/hardware/chip8.cpp
)

find_package(SDL2 REQUIRED)

target_link_libraries(chip_8_emulator PRIVATE SDL2::SDL2) # links all libraries to our chip-8 app
target_include_directories(chip_8_emulator PRIVATE ${SDL2_INCLUDE_DIRS}) # tells CLion where the headers are