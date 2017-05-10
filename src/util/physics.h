/**
 * Based on code at https://gitlab.com/wikibooks-opengl/modern-tutorials/tree/master/glescraft-sdl2
 * which is in the public domain.
 */
#ifndef UTIL_PHYSICS_H
#define UTIL_PHYSICS_H

#include "SDL.h"
#include <glm/glm.hpp>
#include "../world/world.h"
#include "../world/chunk.h"

namespace Game
{
	struct World;
	
	extern glm::vec3 position;
	extern glm::vec3 vec_forward;
	extern glm::vec3 vec_right;
	extern glm::vec3 up;
	extern glm::vec3 lookat;
	extern glm::vec3 angle;

	extern int prev_ticks;
	extern int now;

	void timing();
	void gravity(World* world);
	void movement(unsigned short keys);
	void update_vectors();
}

#endif