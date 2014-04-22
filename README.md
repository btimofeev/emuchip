
### Description

emuChip is cross-platform CHIP-8 and Super CHIP emulator written in C++, Qt and SDL libraries.

### Supported platforms

* Linux
* Windows

### Compilation

To compile Qt version:

1. Open a terminal and browse into the extracted folder.
2. Type 'qmake' to configure the program.
3. Type 'make' to compile the program.
    
To compile SDL version:

1. Open a terminal and browse into the extracted folder.
2. Type 'make -f Makefile.emuchip-sdl' to compile the program.

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

### Author

Copyright (C) 2009-2012 by Boris Timofeev <mashin87@gmail.com>

Site: https://github.com/btimofeev/emuchip

Blog: http://www.emunix.org
