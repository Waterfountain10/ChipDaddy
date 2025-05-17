cmake_minimum_required(VERSION 3.31)
project(chip_8_emulator)

set(CMAKE_CXX_STANDARD 20)

add_executable(chip_8_emulator main.cpp
        src/hardware/chip8.h
        src/hardware/chip8.cpp
)

find_package(SDL2 REQUIRED)

if (SDL2_FOUND)
    target_link_libraries(chip_8_emulator PUBLIC SDL2::SDL2)
else ()
    message(FATAL_ERROR "SDL2 Not Found")
endif ()
