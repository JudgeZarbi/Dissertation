#ifndef WORLD_WORLD_H
#define WORLD_WORLD_H

#define GLM_FORCE_RADIANS
#include "chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Game
{	
	const static int CHUNKS_X = 25;
	const static int CHUNKS_Y = 1;
	const static int CHUNKS_Z = 25;

	struct World
	{
		Chunk* chunks[CHUNKS_X][CHUNKS_Y][CHUNKS_Z];
		Block* colour;

		World();

		void render(GLint coord, GLint mvp, glm::mat4 &pv);
		Block* get(int x, int y, int z) const;
	};
}

#endif