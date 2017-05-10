/**
 * Based on code at https://gitlab.com/wikibooks-opengl/modern-tutorials/tree/master/glescraft-sdl2
 * which is in the public domain.
 */
#include "keyboard.h"

namespace Game
{
	void keyDown(SDL_KeyboardEvent* ev, unsigned short* keys)
	{
		switch(ev->keysym.scancode) {
			case SDL_SCANCODE_A:
				*keys |= 1;
				break;
			case SDL_SCANCODE_D:
				*keys |= 2;
				break;
			case SDL_SCANCODE_W:
				*keys |= 4;
				break;
			case SDL_SCANCODE_S:
				*keys |= 8;
				break;
			case SDL_SCANCODE_SPACE:
				*keys |= 16;
				break;
			case SDL_SCANCODE_C:
				*keys |= 32;
				break;
			default:
				break;
		}
	}

	void keyUp(SDL_KeyboardEvent* ev, unsigned short* keys)
	{
		switch(ev->keysym.scancode) {
			case SDL_SCANCODE_A:
				*keys &= ~1;
				break;
			case SDL_SCANCODE_D:
				*keys &= ~2;
				break;
			case SDL_SCANCODE_W:
				*keys &= ~4;
				break;
			case SDL_SCANCODE_S:
				*keys &= ~8;
				break;
			case SDL_SCANCODE_SPACE:
				*keys &= ~16;
				break;
			case SDL_SCANCODE_C:
				*keys &= ~32;
				break;
			default:
				break;
		}
	}
}
