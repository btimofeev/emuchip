### Description

emuChip is cross-platform CHIP-8 and Super CHIP emulator.

Source code: https://github.com/btimofeev/emuchip

### Compilation

To compile Qt5 version:

1. Open a terminal and browse into the extracted folder.
2. Type `qmake` or `qmake-qt5` to configure the program.
3. Type `make` to compile the program.

To compile SDL version:

1. Open a terminal and browse into the extracted folder.
2. Type `make -f Makefile.emuchip-sdl` to compile the program.

Optionally, you can use CMake build system:

1. Open a terminal and browse into the extracted folder.
2. `mkdir build && cd build`
3. 
    * `cmake ..` to configure Qt version
    * `cmake -D BUILDSDL=1 ..` to configure SDL version
3. `make`

### Key mappings

The original keypad that came with the CHIP-8 computers looked like this:

		+-+-+-+-+
		|1|2|3|C|
		+-+-+-+-+
		|4|5|6|D|
		+-+-+-+-+
		|7|8|9|E|
		+-+-+-+-+
		|A|0|B|F|
		+-+-+-+-+

This is emulated as follows:

		+-+-+-+-+
		|1|2|3|4|
		+-+-+-+-+
		|Q|W|E|R|
		+-+-+-+-+
		|A|S|D|F|
		+-+-+-+-+
		|Z|X|C|V|
		+-+-+-+-+

