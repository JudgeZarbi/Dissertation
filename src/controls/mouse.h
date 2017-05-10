/**
 * Based on code at https://gitlab.com/wikibooks-opengl/modern-tutorials/tree/master/glescraft-sdl2
 * which is in the public domain.
 */
#ifndef CONTROLS_MOUSE_H
#define CONTROLS_MOUSE_H

#include "SDL.h"
#include <glm/glm.hpp>

namespace Game
{
	void mouseMotion(SDL_MouseMotionEvent* ev, glm::vec3* angle);
}

#endif