# chip-8-emulator

Learning C++ by building a CHIP-8 emulator from scratch. No external libraries besides SDL2. Trying out a few ideas along the way.

## what it does

Loads and runs basic CHIP-8 ROMs (like Pong, Breakout, etc). Handles input, rendering, timers, and sound.

## how to build

```bash
git clone https://github.com/your-username/chip-8-emulator.git
cd chip-8-emulator
mkdir build && cd build
cmake ..
make
./chip_8_emulator ../chip8-roms/pong.ch8 10
```

## what’s different

Some personal tweaks:

* computed go-to table for mapping instructions in O(alpha) rather than switch-case's O(logn)
* custom instruction-per-frame (IPF) control (make your game _speedy_ if you want)
* async key handling so input doesn’t block the whole system
* fonts can be loaded from a custom hex address
* some debug-friendly logging for draw calls and memory
* flexible architecture split between platform / chip / gui

## why

Mostly to get more comfortable with C++, memory management, and SDL. 

## Next step is emulating a Gameboy... 
