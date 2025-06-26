# chip-8-emulator

A CHIP-8 emulator/interpreter built using C++. Lightweight and simple. Uses the COSMAC VIP specifications along with modern tweaks. Trying out a few optimizations along the way.

### demo (ROM taken from [here](https://github.com/kripod/chip8-roms/tree/c723a9ed1205a215c5b1e45e994eb54acc243c9e))

<img align="left" src="https://github.com/Waterfountain10/chip-8-emulator/blob/main/src/public/pong.gif" width="50%">
<img align="right" src="https://github.com/Waterfountain10/chip-8-emulator/blob/main/src/public/space-invaders.gif" width="50%">

## what it does

Loads and runs basic, classic CHIP-8 ROMs (like Pong, Breakout, Space Invaders, etc). This allows you to build and play your favorite classic games. The emulator handles input, rendering, timers, and sound.

> **_NOTE:_**  Customizing the `ipf` value allows you to change how fast the ROM runs. Different programs have different preferred values. For a full guide on tuning this value, refer to [this guide](https://tobiasvl.github.io/blog/write-a-chip-8-emulator/#timing).

## how to use 

To use this emulator, you have _two options._

1. Download one of the **release binaries** we have built for each platform (`Windows`, `Linux`, `MacOS`). Refer to our release links here to download. After your download, you can use CLI command `./chip_8_emulator <ROM_path> <ipf>` to run the emulator.

2. Clone this repository and _build the binaries yourself._ Refer to guide below.

## how to build

Any C/C++ compiler should work fine with support of `C++20` or above.

Libraries
- `SDL2`
- `SDL2_image`

```bash
# Example: Ubuntu/Debian
sudo apt-get install libsdl2-2.0-0
```

### Build from this repository:

```bash
git clone https://github.com/Waterfountain10/chip-8-emulator.git
cd chip-8-emulator
mkdir build && cd build
cmake ..
make

# Run your favorite ROM using the format: ./chip_8_emulator <ROM_path> <ipf>
./chip_8_emulator ../chip8-roms/pong.ch8 12
```

## what’s different

Some personal tweaks and optimizations:

* computed go-to table for mapping instructions in O(alpha) rather than switch-case's O(logn)
* custom instruction-per-frame (IPF) control (make your game _speedy_ if you want)
* async key handling so input doesn’t block the whole system
* fonts can be loaded from a custom hex address
* some debug-friendly logging for draw calls and memory
* flexible architecture split between platform / chip / gui

## why

Mostly to get more comfortable with C++, memory management, and SDL. 

## Next step is emulating a Gameboy... 
