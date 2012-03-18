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
#include "chip_emu.h"
#include <cstdlib>
#include <iostream>
#include <fstream>

using namespace std;

ChipEmu::ChipEmu()
{
}

//Clear registers, memory and stack. Load font and redraw screen.
void ChipEmu::init()
{
	PC = 0x200;
	SP = 0;
	I = 0;
	delay_timer = 0;
	sound_timer = 0;

	for (int i = 0; i < 4096; i++)
		memory[i] = 0;
	
	for (int i = 0; i < 2048; i++)
		screen[i] = 0;

	for (int i = 0; i < 16; i++)
		V[i] = 0;
	
	for (int i = 0; i < 16; i++)
		key[i] = 0;

	for (int i = 0; i < 16; i++)
		stack[i] = 0;
	
	unsigned char font[16*5] = {
		0xF0, 0x90, 0x90, 0x90, 0xF0,	// 0
		0x20, 0x60, 0x20, 0x20, 0x70,	// 1
		0xF0, 0x10, 0xF0, 0x80, 0xF0,	// 2
		0xF0, 0x10, 0xF0, 0x10, 0xF0,	// 3
		0x90, 0x90, 0xF0, 0x10, 0x10,	// 4
		0xF0, 0x80, 0xF0, 0x10, 0xF0,	// 5
		0xF0, 0x80, 0xF0, 0x90, 0xF0,	// 6
		0xF0, 0x10, 0x20, 0x40, 0x40,	// 7
		0xF0, 0x90, 0xF0, 0x90, 0xF0,	// 8
		0xF0, 0x90, 0xF0, 0x10, 0xF0,	// 9
		0xF0, 0x90, 0xF0, 0x90, 0x90,	// A
		0xE0, 0x90, 0xE0, 0x90, 0xE0,	// B
		0xF0, 0x80, 0x80, 0x80, 0xF0,	// C
		0xE0, 0x90, 0x90, 0x90, 0xE0,	// D
		0xF0, 0x80, 0xF0, 0x80, 0xF0,	// E
		0xF0, 0x80, 0xF0, 0x80, 0x80	// F
	};
	//Load font
	for (int i = 0; i < 16*5; i++)
		memory[i] = font[i];
	
	need_redraw = true;
}

void ChipEmu::drawSprite(unsigned char X, unsigned char Y, unsigned char N)
{	
	V[0xF] = 0;
	for (int yline = 0; yline < N; yline++)
	{
		unsigned char data = memory[I + yline];
		for (int xpix = 0; xpix < 8; xpix++)
		{
			if((data & (0x80 >> xpix)) != 0)
			{
				if (screen[V[X] + xpix + (V[Y] + yline) * 64] == 1) V[0xF] = 1;
				screen[V[X] + xpix + (V[Y] + yline) * 64] ^= 1;
				//screen[V[X] + xpix + (V[Y] + yline) * 64] = (screen[V[X] + xpix + (V[Y] + yline) * 64] || 1) && !(screen[V[X] + xpix + (V[Y] + yline) * 64] && 1);
			}
		}
	}
}

void ChipEmu::executeNextOpcode()
{
	
	int i, n;

	opcode = (memory[PC]<<8) + memory[PC+1];

	PC += 2;

	switch ((opcode & 0xF000)>>12)
	{
		case 0x0:
			switch	(opcode & 0x00FF)
			{
				case 0xE0:		// 00E0 - clear the screen
					for (int i = 0; i < 2048; i++)
						screen[i]=0;
					need_redraw = true;
					break;

				case 0xEE:		// 00EE - return from subroutine
					PC = stack[--SP];
					break;
				
				default:
					cerr << "Unknown opcode: 0x" << hex << opcode <<endl;
			}
			break;

		case 0x1:				// 1NNN - jump to addr
			PC = opcode & 0x0FFF;
			break;

		case 0x2:				// 2NNN - call subroutine
			stack[SP++] = PC;
			PC = opcode & 0x0FFF;
			break;

		case 0x3:				// 3XKK - skip next instruction if VX == Byte
			if (V[((opcode & 0x0F00)>>8)] == (opcode & 0x00FF)) PC += 2;
			break;

		case 0x4:				// 4XKK - skip next instruction if VX != Byte
			if (V[((opcode & 0x0F00)>>8)] != (opcode & 0x00FF)) PC += 2;
			break;

		case 0x5:				// 5XY0 - skip next instruction if VX == VY
			if (V[((opcode & 0x0F00)>>8)] == V[((opcode & 0x00F0)>>4)]) PC += 2;
			break;

		case 0x6:				// 6XKK - set VX = Byte
			V[((opcode & 0x0F00)>>8)] = opcode & 0x00FF;
			break;

		case 0x7:				// 7XKK - set VX = VX + Byte
			V[((opcode & 0x0F00)>>8)] += opcode & 0x00FF;
			break;

		case 0x8:
			switch (opcode & 0x000F)
			{
				case 0x0:		// 8XY0 - set VX = VY
					V[((opcode & 0x0F00)>>8)] = V[((opcode & 0x00F0)>>4)];
					break;

				case 0x1:		// 8XY1 - set VX = VX | VY
					V[((opcode & 0x0F00)>>8)] |= V[((opcode & 0x00F0)>>4)];
					break;

				case 0x2:		// 8XY2 - set VX = VX & VY
					V[((opcode & 0x0F00)>>8)] &= V[((opcode & 0x00F0)>>4)];
					break;

				case 0x3:		// 8XY3 - set VX = VX ^ VY
					V[((opcode & 0x0F00)>>8)] ^= V[((opcode & 0x00F0)>>4)];
					break;

				case 0x4:		// 8XY4 - set VX = VX + VY, VF = carry
					i = static_cast<int>(V[((opcode & 0x0F00)>>8)]) + static_cast<int>(V[((opcode & 0x00F0)>>4)]);

					if (i > 255)
						V[0xF] = 1;
					else
						V[0xF] = 0;

					V[((opcode & 0x0F00)>>8)] = i;
					break;

				case 0x5:		// 8XY5 - set VX = VX - VY, VF = !borrow
					if (V[((opcode & 0x0F00)>>8)] >= V[((opcode & 0x00F0)>>4)])
						V[0xF] = 1;
					else
						V[0xF] = 0;

					V[((opcode & 0x0F00)>>8)] -= V[((opcode & 0x00F0)>>4)];
					break;

				case 0x6:		// 8XY6 - set VX = VX >> 1, VF = carry
					V[0xF] = V[((opcode & 0x0F00)>>8)] & 0x1;
					V[((opcode & 0x0F00)>>8)] >>= 1;
					break;

				case 0x7:		// 8XY7 - set VX = VY - VX, VF = !borrow
					if (V[((opcode & 0x00F0)>>4)] >= V[((opcode & 0x0F00)>>8)])
						V[0xF] = 1;
					else
						V[0xF] = 0;

					V[((opcode & 0x0F00)>>8)] = V[((opcode & 0x00F0)>>4)] - V[((opcode & 0x0F00)>>8)];
					break;

				case 0xE:		// 8XYE - set VX = VX << 1, VF = carry
					V[0xF] = (V[((opcode & 0x0F00)>>8)] >> 7) & 0x01;
					V[((opcode & 0x0F00)>>8)] <<= 1;
					break;
				
				default:
					cerr << "Unknown opcode: 0x" << hex << opcode <<endl;
			}
			break;

		case 0x9:				// 9XY0 - skip next instruction if VX != VY
			if (V[((opcode & 0x0F00)>>8)] != V[((opcode & 0x00F0)>>4)]) PC += 2;
			break;

		case 0xA:				// ANNN - set I = Addr
			I = opcode & 0x0FFF;
			break;

		case 0xB:				// BNNN - jump to Addr + V0
			PC = (opcode & 0x0FFF) + V[0];
			break;

		case 0xC:				// CXKK - set VX = random & Byte
			V[((opcode & 0x0F00)>>8)] = rand() & (opcode & 0x00FF);
			break;

		case 0xD:				// DXYN - Draw sprite
			drawSprite(((opcode & 0x0F00)>>8), ((opcode & 0x00F0)>>4), (opcode & 0x000F));
			need_redraw = true;
			break;

		case 0xE:
			switch (opcode & 0x00FF)
			{
				case 0x9E:		// EX9E - skip next instruction if key VX down
					if (key[V[((opcode & 0x0F00)>>8)]] == 1)
						PC += 2;
					break;

				case 0xA1:		// EXA1 - skip next instruction if key VX up
					if (key[V[((opcode & 0x0F00)>>8)]] == 0)
						PC += 2;
					break;
					
				default:
					cerr << "Unknown opcode: 0x" << hex << opcode <<endl;
			}
			break;

		case 0xF:
			switch (opcode & 0x00FF)
			{
				case 0x07:		// FX07 - set VX = delaytimer
					V[((opcode & 0x0F00)>>8)] = delay_timer;
					break;

				case 0x0A:		// FX0A - set VX = key, wait for keypress
					for (n=0; n < 16; n++)
					{
						if (key[n] == 1)
						{
							V[((opcode & 0x0F00)>>8)] = n;
							break;
						}
					}
					PC -= 2;
					break;

				case 0x15:		// FX15 - set delaytimer = VX
					delay_timer = V[((opcode & 0x0F00)>>8)];
					break;

				case 0x18:		// FX18 - set soundtimer = VX
					sound_timer = V[((opcode & 0x0F00)>>8)];
					break;

				case 0x1E:		// FX1E - set I = I + VX
					I += V[((opcode & 0x0F00)>>8)];
					break;

				case 0x29:		// FX29 - point I to 5 byte numeric sprite for value in VX
					I = V[((opcode & 0x0F00)>>8)] * 5;
					break;

				case 0x33:		// FX33 - store BCD of VX in [I], [I+1], [I+2]
					n = V[((opcode & 0x0F00)>>8)];
					memory[I] = (n - (n % 100)) / 100;
					n -= memory[I] * 100;
					memory[I+1] = (n - (n % 10)) / 10;
					n -= memory[I+1] * 10;
					memory[I+2] = n;
					break;

				case 0x55:		// FX55 - store V0 .. VX in [I] .. [I+X]
					for (n=0; n <= ((opcode & 0x0F00)>>8); n++)
						memory[I++] = V[n];
					break;

				case 0x65:		// FX65 - read V0 .. VX from [I] .. [I+X]
					for (n=0; n <= ((opcode & 0x0F00)>>8); n++)
						V[n] = memory[I++];
					break;
					
				default:
					cerr << "Unknown opcode: 0x" << hex << opcode <<endl;
			}
			break;

		default:
			cerr << "Unknown opcode: 0x" << hex << opcode <<endl;
	}
}

bool ChipEmu::loadGame(const char *filename)
{
	ifstream file(filename, ios::in | ios::binary | ios::ate);
	if (file.is_open())
	{
		ifstream::pos_type size;
		size = file.tellg();
		if (size > 0x0FFF - 0x200)
		{
			cerr << "Error: file '" << filename << "' is too large." << endl;
			return false;
		}
		file.seekg(0, ios::beg);
		file.read(reinterpret_cast<char*>(&memory[0x200]), size);
		file.close();
		
		cout << "File '" << filename << "' loaded." << endl;
		return true;
	}

	cerr << "Error: unable to open file '" << filename << "'" << endl;
	return false;
}

void ChipEmu::decreaseTimers()
{
	if(delay_timer > 0)
		--delay_timer;

	if(sound_timer > 0)
		--sound_timer;
}
