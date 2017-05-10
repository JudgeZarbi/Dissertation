/**
 * Based on code at https://gitlab.com/wikibooks-opengl/modern-tutorials/tree/master/glescraft-sdl2
 * which is in the public domain.
 */
#include "physics.h"

namespace Game
{
	int prev_ticks = 0;
	int now = 0;

	glm::vec3 position = glm::vec3(0,0,0);
	glm::vec3 vec_forward = glm::vec3(0,0,0);
	glm::vec3 vec_right = glm::vec3(0,0,0);
	glm::vec3 up = glm::vec3(0,0,0);
	glm::vec3 lookat = glm::vec3(0,0,0);
	glm::vec3 angle = glm::vec3(0,0,0);

	void timing()
	{
		prev_ticks = now;
		now = SDL_GetTicks();
	}

	void movement(unsigned short keys)
	{
		static const float movespeed = 10;
		float dt = (now - prev_ticks) * 1.0e-3;

		if(keys & 1)
		{
			position -= vec_right * movespeed * dt;
		}
		if(keys & 2)
		{
			position += vec_right * movespeed * dt;
		}
		if(keys & 4)
		{
			position += vec_forward * movespeed * dt;
		}
		if(keys & 8)
		{
			position -= vec_forward * movespeed * dt;
		}
		if(keys & 16)
		{
			position.y += 2 * movespeed * dt;
		}
//		if(keys & 32)
//		{
//			position.y -= movespeed * dt;
//		}
	}

	void gravity(World* world)
	{
		static const float movespeed = 10;
		float dt = (now - prev_ticks) * 1.0e-3;

		float fx = floor(position.x);
		float fz = floor(position.z);

		position.y -= movespeed*dt;

		int block_y = 0;
		for(int y = Y - 1; y >= 0; y--)
		{
			if (world->get(fx, y, fz))
			{
				block_y = y;
				break;
			}
		}

		//This kind of results in some hilarious auto-jump mechanics
		if(block_y + 1 > position.y - 1.8)
		{
			position.y = block_y + 2.8;
		}
	}

	// Calculate the vec_forward, vec_right and lookat vectors from the angle vector
	void update_vectors()
	{
		vec_forward.x = sinf(angle.x);
		vec_forward.y = 0;
		vec_forward.z = cosf(angle.x);

		vec_right.x = -cosf(angle.x);
		vec_right.y = 0;
		vec_right.z = sinf(angle.x);

		lookat.x = sinf(angle.x) * cosf(angle.y);
		lookat.y = sinf(angle.y);
		lookat.z = cosf(angle.x) * cosf(angle.y);

		up = glm::cross(vec_right, lookat);
	}
}