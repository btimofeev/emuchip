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
SDL_Surface *sprite_surf;

ChipEmu emu;
bool done;

bool gfx_init()
{
	// Initializes the video subsystem
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		return false;
	}

	SDL_WM_SetCaption("emuchip-sdl", "emuchip-sdl");
	
	if((screen_surf = SDL_SetVideoMode(320, 160, 24, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL) {
		return false;
	}
	
	if((sprite_surf = SDL_LoadBMP("sprite.bmp")) == NULL)
		return false;
	
	return true;
}

void draw()
{
	SDL_FillRect(screen_surf, NULL, 0x000000);
	int t = 0;
	for (int i = 0; i < 32; i++)
		for (int j = 0; j < 64; j++)
		{
			if (emu.screen[t++] == 1)
			{
				SDL_Rect dest;
				dest.x = j*5;
				dest.y = i*5;
				SDL_BlitSurface(sprite_surf, NULL, screen_surf, &dest);
			}
		}
	emu.need_redraw = false;
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
		
		while (!done)
		{
			//keyboard
			while (SDL_PollEvent(&event)) {
				handle_input(event);
			}
			
			//redraw screen
			if(emu.need_redraw)
			{	
				draw();
				SDL_Flip(screen_surf);
			}
			
			emu.executeNextOpcode();
			
			//decrease timers every 1/60sec
			if (SDL_GetTicks() - lasttick >= 1000/60)
			{
				emu.decreaseTimers();
				lasttick = SDL_GetTicks();
			}
		}
	}
	
	SDL_FreeSurface(sprite_surf);
	SDL_FreeSurface(screen_surf);
	SDL_Quit;
	return 0;
}
