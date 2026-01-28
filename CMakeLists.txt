cmake_minimum_required(VERSION 3.20)
project(chip_8_emulator)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(chip_8_emulator
        main.cpp
        src/hardware/chip.h
        src/hardware/chip.cpp
        src/hardware/instructions.cpp
        src/hardware/instructions.h
        src/Platform.cpp
        src/Platform.h
        src/gui/gui.cpp
        src/gui/gui.h
        src/hardware/bench_dispatch.cpp
        src/hardware/bench_dispatch.h
)

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)

target_link_libraries(chip_8_emulator PRIVATE
        SDL2::SDL2
        SDL2_image::SDL2_image
        SDL2_mixer::SDL2_mixer
)

# for BenchMarking Dispatch table Configuration:
add_executable(dispatch_benchmark
        src//hardware/bench_dispatch.cpp
        src/hardware/instructions.cpp
        src/hardware/chip.cpp
)

target_include_directories(dispatch_benchmark PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(dispatch_benchmark PRIVATE ${SDL2_LIBRARIES})

