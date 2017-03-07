#ifndef WORLD_CHUNK_H
#define WORLD_CHUNK_H

#include <GL/glew.h>
#include "SDL.h"
#include "../util/simplex.h"
#include "../util/vertex.h"
#include "../block/block.h"
#include "../block/rcblock.h"
#include "../block/colourblock.h"

namespace Game
{
	const static int X = 16;
	const static int Y = 256;
	const static int Z = 16;

	struct Chunk
	{
		Block* voxel[X][Y][Z];
		GLuint vbo;
		int elements;
		bool init;
		bool changed;
		Chunk* l;
		Chunk* r;
		Chunk* b;
		Chunk* f;
		Chunk* u;
		Chunk* d;
		int cx;
		int cy;
		int cz;
		vertex* vertices;

		Chunk(int cx, int cy, int cz);

		void initialise();
		void build_vertices();
		bool is_visible(int x, int y, int z, int xadj, int yadj, int zadj);
		Block* get_block(int x, int y, int z);

	};

}

#endif