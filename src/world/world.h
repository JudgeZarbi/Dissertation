/**
 * Based on code at https://gitlab.com/wikibooks-opengl/modern-tutorials/tree/master/glescraft-sdl2
 * which is in the public domain.
 */
#ifndef WORLD_WORLD_H
#define WORLD_WORLD_H

#define GLM_FORCE_RADIANS
#include "chunk.h"
#include "../util/physics.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../threads/worldgenthread.h"
#include "../util/global.h"

namespace Game
{	

	//hopefully temp
	struct WorldGenThread;

	struct World
	{
		Chunk* chunks[CHUNKS_X][CHUNKS_Y][CHUNKS_Z];
		int x_max;
		int z_max;
		int x_mpos;
		int z_mpos;
		int x_load = CHUNKS_X - CHUNKS_RANGE - FRAME_RANGE - VERT_RANGE;
		int z_load = CHUNKS_Z - CHUNKS_RANGE - FRAME_RANGE - VERT_RANGE;
		const int load_dist = (FRAME_RANGE + VERT_RANGE) * 2 + 1;
		int x_old = 0;
		int z_old = 0;

		World();
		virtual ~World();

		Block* get(int x, int y, int z) const;
		Chunk* get_chunk(int x, int y, int z) const;
		void move(WorldGenThread** wg_threads);
		void consistency();

		void print(std::ostream& out);

	};
}

#endif