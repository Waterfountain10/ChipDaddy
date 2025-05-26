cmake_minimum_required(VERSION 3.31)
project(chip_8_emulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(chip_8_emulator main.cpp
        src/hardware/chip.h
        src/hardware/chip.cpp
        src/hardware/instructions.cpp
        src/hardware/instructions.h
        src/Platform.cpp
        src/Platform.h
        src/GUI/GUI.cpp
        src/GUI/GUI.h
)

find_package(SDL2 REQUIRED)

target_compile_features(chip_8_emulator
        PRIVATE
        cxx_std_20
)

# For Mac with brew-installed SDL2_image
if(APPLE)
    if(EXISTS "/opt/homebrew/Cellar/sdl2_image")
        include_directories("/opt/homebrew/include")
        target_link_directories(chip_8_emulator PRIVATE "/opt/homebrew/lib")
        target_link_libraries(chip_8_emulator PRIVATE SDL2 SDL2_image)
    endif()
endif()

# For all the other types of operating systems install the default
include_directories(${SDL2_INCLUDE_DIRS})

target_link_libraries(chip_8_emulator
        PRIVATE
        SDL2::SDL2
        SDL2::SDL2main
) # links all libraries to our chip-8 app
target_include_directories(chip_8_emulator
        PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2_IMAGE_INCLUDE_DIRS}
) # tells CLion where the headers are)
