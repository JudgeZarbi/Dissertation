#ifndef _JZ_MOUSE_H
#define _JZ_MOUSE_H

#include "SDL.h"
#include <glm/glm.hpp>

namespace Controls
{
	void mouseMotion(SDL_MouseMotionEvent* ev, glm::vec3* angle);
}

#endif