/**
 * Based on code at https://gitlab.com/wikibooks-opengl/modern-tutorials/tree/master/glescraft-sdl2
 * which is in the public domain.
 */
#ifndef WORLD_CHUNK_H
#define WORLD_CHUNK_H

#include <string>
#include <sstream>
#include <GL/glew.h>
#include "SDL.h"
#include "../util/simplex.h"
#include "../util/vertex.h"
#include "../util/shader_utils.h"
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
		GLuint vbo = 0;
		int elements;
		bool init;
		bool changed;
		bool vert_ready = false;
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
		virtual ~Chunk();

		void initialise();
		void build_vertices();
		bool is_visible(int x, int y, int z, int xadj, int yadj, int zadj);
		Block* get_block(int x, int y, int z);
		void clear_vertices();
		void initialise_from_file();

	};

}

#endif