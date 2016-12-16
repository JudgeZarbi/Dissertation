#include "physics.h"

namespace Util
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
			position.y += movespeed * dt;
		}
		if(keys & 32)
		{
			position.y -= movespeed * dt;
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