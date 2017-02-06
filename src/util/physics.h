#ifndef UTIL_PHYSICS_H
#define UTIL_PHYSICS_H

#include "SDL.h"
#include <glm/glm.hpp>

namespace Game
{
	extern glm::vec3 position;
	extern glm::vec3 vec_forward;
	extern glm::vec3 vec_right;
	extern glm::vec3 up;
	extern glm::vec3 lookat;
	extern glm::vec3 angle;

	extern int prev_ticks;
	extern int now;

	void timing();
	void movement(unsigned short keys);
	void update_vectors();
}

#endif