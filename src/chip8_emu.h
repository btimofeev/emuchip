/*
 *  emuChip - CHIP-8 emulator.
 *  Copyright (C) 2009  Boris Timofeev <mashin87@gmail.com>
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

#ifndef CHIP8_EMU_H
#define CHIP8_EMU_H

class Chip8_Emu
{
public:
	Chip8_Emu();
	int cpuEmul();
	void emuReset();
	
	void setMemory(unsigned char [0x0FFF]);
	void setDelay(unsigned char d)
		{delay = d;}
	void setSound(unsigned char s)
		{sound = s;}
	void setKey(int keyNumber, unsigned short value)
		{keys[keyNumber] = value;}
	
	unsigned char getDelay()
		{return delay;}
	unsigned char getSound()
		{return sound;}
	unsigned short getOpcode()
		{return opcode;}
	
	unsigned char screen[64*32];

private:
	unsigned short opcode;

	unsigned char V[16];			// general purpose registers & VF carry flag
	unsigned short I;				// adress register
	unsigned short PC;				// program counter
	unsigned char SP;				// stack pointer
	
	unsigned short stack[16];
	
	unsigned char memory[0x0FFF];	


	unsigned char delay;
	unsigned char sound;
	
	unsigned short keys[16];		// 0 - KEY_UP
									// 1 - KEY_PRESSED
									// 2 - KEY_DOWN
									// 3 - KEY_RELEASED
										
	void drawSprite(unsigned char X, unsigned char Y, unsigned char N);
};

#endif
