/*
 *  emuChip - CHIP-8 emulator.
 *  Copyright (C) 2009-2012 Boris Timofeev <mashin87@gmail.com> <http://www.emunix.org>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef CHIP_EMU_H
#define CHIP_EMU_H

#include <string>

class ChipEmu
{
private:
	unsigned short opcode;
	unsigned char V[16];			// general purpose registers & VF carry flag
	unsigned short I;				// adress register
	unsigned short PC;				// program counter
	unsigned char SP;				// stack pointer
	
	unsigned short stack[16];
	
	unsigned char memory[4096];	

	unsigned char delay_timer;
	unsigned char sound_timer;

	void drawSprite(unsigned char X, unsigned char Y, unsigned char N);

public:
	unsigned char screen[64*32];
	unsigned char key[16];			// 0 - KEY_RELEASED
									// 1 - KEY_PRESSED

	ChipEmu();
	void init();
	bool loadGame(const char *filename);
	void executeNextOpcode();
	void decreaseTimers();
	
};
#endif
