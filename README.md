### Description

emuChip is cross-platform CHIP-8 and Super CHIP emulator.

Source code: https://github.com/btimofeev/emuchip

### Compilation

1. Open a terminal and browse into the extracted folder.
2. `mkdir build && cd build`
3. 
    * `cmake ..` to configure Qt and SDL versions, or
    * `cmake -D BUILD_QT=NO ..` to disable QT version
    * `cmake -D BUILD_SDL=NO ..` to disable SDL version
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

