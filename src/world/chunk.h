#ifndef _JZ_CHUNK_H
#define _JZ_CHUNK_H

#include <GL/glew.h>
#include "SDL.h"
#include "../util/simplex.h"
#include "../util/vertex.h"
#include "../block/block.h"

namespace World
{
	const static int X = 16;
	const static int Y = 256;
	const static int Z = 16;

	struct Chunk
	{
		Block::Block voxel[X][Y][Z];
		GLuint vbo;
		int elements;
		bool init;
		Chunk* l;
		Chunk* r;
		Chunk* b;
		Chunk* f;
		Chunk* u;
		Chunk* d;
		int cx;
		int cy;
		int cz;

		Chunk();

		void initialise(int cx, int cz);
		void build_vertices();
		bool is_visible(int x, int y, int z, int xadj, int yadj, int zadj);
		void render(GLint coord);
		Block::Blocks get_block(int x, int y, int z);

	};
}
#endif