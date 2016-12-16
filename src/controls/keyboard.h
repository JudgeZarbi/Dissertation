#ifndef _JZ_KEYBOARD_H
#define _JZ_KEYBOARD_H

#include "SDL.h"

namespace Controls
{	
	void keyDown(SDL_KeyboardEvent* ev, unsigned short* keys);
	void keyUp(SDL_KeyboardEvent* ev, unsigned short* keys);
}

#endif