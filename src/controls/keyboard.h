/**
 * Based on code at https://gitlab.com/wikibooks-opengl/modern-tutorials/tree/master/glescraft-sdl2
 * which is in the public domain.
 */
#ifndef CONTROLS_KEYBOARD_H
#define _CONTROLS_KEYBOARD_H

#include "SDL.h"

namespace Game
{	
	void keyDown(SDL_KeyboardEvent* ev, unsigned short* keys);
	void keyUp(SDL_KeyboardEvent* ev, unsigned short* keys);
}

#endif