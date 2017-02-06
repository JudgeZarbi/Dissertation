#ifndef CONTROLS_KEYBOARD_H
#define _CONTROLS_KEYBOARD_H

#include "SDL.h"

namespace Game
{	
	void keyDown(SDL_KeyboardEvent* ev, unsigned short* keys);
	void keyUp(SDL_KeyboardEvent* ev, unsigned short* keys);
}

#endif