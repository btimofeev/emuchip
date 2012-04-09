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
#include <SDL/SDL.h>
#include <iostream>
#include "../chip_emu.h"

using namespace std;

SDL_Surface *screen_surf;

ChipEmu emu;
bool done;
int opcode_count;

bool gfx_init()
{
	// Initializes the video subsystem
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		return false;
	}

	SDL_WM_SetCaption("emuchip-sdl", "emuchip-sdl");
	
	if((screen_surf = SDL_SetVideoMode(256, 128, 24, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		return false;
	}
	
	return true;
}

void draw()
{
	SDL_FillRect(screen_surf, NULL, 0x000000);
	int t = 0;
	for (int y = 0; y < 64; y++)
		for (int x = 0; x < 128; x++)
		{
			if (emu.screen[x][y] == 1)
			{
				SDL_Rect dest;
				dest.x = x*2;
				dest.y = y*2;
				dest.w = 2;
				dest.h = 2;
				SDL_FillRect(screen_surf, &dest, SDL_MapRGB(screen_surf->format, 0xff, 0xff, 0xff));
			}
		}
}

void handle_input(SDL_Event &event)
{
	switch(event.type){
		case SDL_KEYDOWN:
			switch(event.key.keysym.sym){
				case SDLK_1: emu.key[0x1]=1; break;
				case SDLK_2: emu.key[0x2]=1; break;
				case SDLK_3: emu.key[0x3]=1; break;
				case SDLK_4: emu.key[0xc]=1; break;
				case SDLK_q: emu.key[0x4]=1; break;
				case SDLK_w: emu.key[0x5]=1; break;
				case SDLK_e: emu.key[0x6]=1; break;
				case SDLK_r: emu.key[0xd]=1; break;
				case SDLK_a: emu.key[0x7]=1; break;
				case SDLK_s: emu.key[0x8]=1; break;
				case SDLK_d: emu.key[0x9]=1; break;
				case SDLK_f: emu.key[0xe]=1; break;
				case SDLK_z: emu.key[0xa]=1; break; 
				case SDLK_x: emu.key[0x0]=1; break;
				case SDLK_c: emu.key[0xb]=1; break;
				case SDLK_v: emu.key[0xf]=1; break;
			}
			break;
		case SDL_KEYUP:
			switch(event.key.keysym.sym){
				case SDLK_1: emu.key[0x1]=0; break;
				case SDLK_2: emu.key[0x2]=0; break;
				case SDLK_3: emu.key[0x3]=0; break;
				case SDLK_4: emu.key[0xc]=0; break;
				case SDLK_q: emu.key[0x4]=0; break;
				case SDLK_w: emu.key[0x5]=0; break;
				case SDLK_e: emu.key[0x6]=0; break;
				case SDLK_r: emu.key[0xd]=0; break;
				case SDLK_a: emu.key[0x7]=0; break;
				case SDLK_s: emu.key[0x8]=0; break;
				case SDLK_d: emu.key[0x9]=0; break;
				case SDLK_f: emu.key[0xe]=0; break;
				case SDLK_z: emu.key[0xa]=0; break; 
				case SDLK_x: emu.key[0x0]=0; break;
				case SDLK_c: emu.key[0xb]=0; break;
				case SDLK_v: emu.key[0xf]=0; break;
			}
			break;
		case SDL_QUIT:
			done = true;
			break;
	}
}

int main(int argc, char *argv[])
{
	cout << "emuChip-sdl v0.3" << endl << "Copyright (C) 2009-2012 Boris Timofeev <mashin87@gmail.com>" << endl << endl;
	if (argc == 1){
		cerr << "Error: no input file specified" << endl;
		cerr << "Usage: " << argv[0] << " FILE" << endl;
		return 1;
	}
	
	if (gfx_init() == false)
	{
		cerr << "Unable to initialize SDL: " << SDL_GetError() << endl;
		exit(1);
	}
	
	emu.init();
	if (emu.loadGame(argv[1])){
		SDL_Event event;
		done = false;
		Uint32 lasttick = SDL_GetTicks(); //for delay fps
		
		int opcode_count = 0;
		while (!done)
		{
			//keyboard
			while (SDL_PollEvent(&event)) {
				handle_input(event);
			}
		
			if (opcode_count < 12)	// execute 720 opcodes per sec
			{
				emu.executeNextOpcode();
				opcode_count++;
			}

			//decrease timers every 1/60sec and redraw screen
			if (SDL_GetTicks() - lasttick >= 1000/60)
			{
				emu.decreaseTimers();
				lasttick = SDL_GetTicks();
				
				draw();
				SDL_Flip(screen_surf);
				
				opcode_count = 0;
			}
			
			if (emu.stop) done = true;
		}
	}
	
	SDL_FreeSurface(screen_surf);
	SDL_Quit;
	return 0;
}
