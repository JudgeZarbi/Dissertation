#ifndef _JZ_WORLD_H
#define _JZ_WORLD_H

#define GLM_FORCE_RADIANS
#include "chunk.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace World
{	
	const static int CHUNKS_X = 25;
	const static int CHUNKS_Y = 1;
	const static int CHUNKS_Z = 25;

	struct World
	{
		Chunk* chunks[CHUNKS_X][CHUNKS_Y][CHUNKS_Z];
		Block::Block* colour;

		World();

		void render(GLint coord, GLint mvp, glm::mat4 &pv);

		Block::Block getBlock(int x, int y, int z);
	};
}

#endif