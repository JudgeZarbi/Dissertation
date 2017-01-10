#include "chunk.h"

namespace World
{
	Chunk::Chunk()
	{
		memset(voxel, 0, sizeof(voxel));
		l = r = b = f = u = d = 0;
		init = false;
	}

	//4D generation, no x listed, but is same as 3D

	// 		for(int y = 0; y < Y; y++)
	// 		{
	// 		for(int z = 0; z < Z; z++)
	// 		{
	// 			double noise = scaled_octave_noise_4d(10, 0.1, 1, 0.2, 0.75, (cx*16+x)/32.0, y/32.0, (cz*16+z)/32.0, 0);
	// 			double max_y = noise * 32;
	// //			for (int y = 0; y < max_y; y++)
	// //			{
	// //				voxel[x][y][z] = 1;
	// //			}

	// 				if (noise > 0.5)
	// 				{
	// 					voxel[x][y][z] = 1;
	// 				}
	// 		}
				
	// 		}


	void Chunk::initialise(int cx, int cz)
	{
		for(int x = 0; x < X; x++)
		{
			for(int z = 0; z < Z; z++)
			{
				double noise = Util::scaled_octave_noise_3d(10, 0.25, 1, 0.4, 0.6, (cx*16+x)/128.0, (cz*16+z)/128.0, 0);
				double max_y = noise * Y;
				for (int y = 0; y < max_y - 5; y++)
				{
					voxel[x][y][z] = Block::Block(3);
				}
				for (int y = max_y-5; y < max_y - 1; y++)
				{
					voxel[x][y][z] = Block::Block(2);
				}
				int y = max_y - 1;
				voxel[x][y][z] = Block::Block(1);
			}
		}
		init = true;
	}

	void Chunk::build_vertices()
	{
		Util::vertex* vertices = new Util::vertex[X * Y * Z * 36];
		int i = 0;
		// Cube left side
		for(int x = X - 1; x >= 0; x--) 
		{
			for(int y = 0; y < Y; y++) 
			{
				for(int z = 0; z < Z; z++) 
				{
					if(!is_visible(x, y, z, x-1, y, z))
					{
						continue;
					}

					uint8_t side = voxel[x][y][z].type;
 
					vertices[i++] = Util::vertex(x, y, z, side);
					vertices[i++] = Util::vertex(x, y, z + 1, side);
					vertices[i++] = Util::vertex(x, y + 1, z, side);
					vertices[i++] = Util::vertex(x, y + 1, z, side);
					vertices[i++] = Util::vertex(x, y, z + 1, side);
					vertices[i++] = Util::vertex(x, y + 1, z + 1, side);
				}
			}
		}
		// Cube right side
		for(int x = 0; x < X; x++) 
		{
			for(int y = 0; y < Y; y++) 
			{
				for(int z = 0; z < Z; z++) 
				{
					if(!is_visible(x, y, z, x+1, y, z))
					{
						continue;
					}

					uint8_t side = voxel[x][y][z].type;
				
					vertices[i++] = Util::vertex(x + 1, y, z + 1, side);
					vertices[i++] = Util::vertex(x + 1, y, z, side);
					vertices[i++] = Util::vertex(x + 1, y + 1, z + 1, side);
					vertices[i++] = Util::vertex(x + 1, y + 1, z + 1, side);
					vertices[i++] = Util::vertex(x + 1, y, z, side);
					vertices[i++] = Util::vertex(x + 1, y + 1, z, side);
				}
			}
		}

		// Cube bottom side
		for(int x = 0; x < X; x++) 
		{
			for(int y = Y - 1; y >= 0; y--) 
			{
				for(int z = 0; z < Z; z++) 
				{
					if(!is_visible(x, y, z, x, y-1, z))
					{
						continue;
					}

					uint8_t bottom = voxel[x][y][z].type;

					if (bottom == 1)
					{
						bottom = 2;
					}

					vertices[i++] = Util::vertex(x, y, z, bottom);
					vertices[i++] = Util::vertex(x + 1, y, z, bottom);
					vertices[i++] = Util::vertex(x, y, z + 1, bottom);
					vertices[i++] = Util::vertex(x + 1, y, z, bottom);
					vertices[i++] = Util::vertex(x + 1, y, z + 1, bottom);
					vertices[i++] = Util::vertex(x, y, z + 1, bottom);
				}
			}
		}
		// Cube top side
		for(int x = 0; x < X; x++) 
		{
			for(int y = 0; y < Y; y++) 
			{
				for(int z = 0; z < Z; z++) 
				{
					if(!is_visible(x, y, z, x, y+1, z))
					{
						continue;
					}

					uint8_t top = voxel[x][y][z].type;

					if (top == 1)
					{
						top = 4;
					}
					
					vertices[i++] = Util::vertex(x, y + 1, z, top);
					vertices[i++] = Util::vertex(x, y + 1, z + 1, top);
					vertices[i++] = Util::vertex(x + 1, y + 1, z, top);
					vertices[i++] = Util::vertex(x + 1, y + 1, z, top);
					vertices[i++] = Util::vertex(x, y + 1, z + 1, top);
					vertices[i++] = Util::vertex(x + 1, y + 1, z + 1, top);
				}
			}
		}

		// Cube back side
		for(int x = 0; x < X; x++) 
		{
			for(int y = 0; y < Y; y++) 
			{
				for(int z = Z - 1; z >= 0; z--) 
				{
					if(!is_visible(x, y, z, x, y, z-1))
					{
						continue;
					}

					uint8_t side = voxel[x][y][z].type;

					vertices[i++] = Util::vertex(x + 1, y, z, side);
					vertices[i++] = Util::vertex(x, y, z, side);
					vertices[i++] = Util::vertex(x + 1, y + 1, z, side);
					vertices[i++] = Util::vertex(x + 1, y + 1, z, side);
					vertices[i++] = Util::vertex(x, y, z, side);
					vertices[i++] = Util::vertex(x, y + 1, z, side);
				}
			}
		}
		// Cube front side
		for(int x = 0; x < X; x++) 
		{
			for(int y = 0; y < Y; y++) 
			{
				for(int z = 0; z < Z; z++) 
				{
					if(!is_visible(x, y, z, x, y, z+1))
					{
						continue;
					}
					
					uint8_t side = voxel[x][y][z].type;

					vertices[i++] = Util::vertex(x, y, z + 1, side);
					vertices[i++] = Util::vertex(x + 1, y, z + 1, side);
					vertices[i++] = Util::vertex(x, y + 1, z + 1, side);
					vertices[i++] = Util::vertex(x, y + 1, z + 1, side);
					vertices[i++] = Util::vertex(x + 1, y, z + 1, side);
					vertices[i++] = Util::vertex(x + 1, y + 1, z + 1, side);
				}
			}
		}

		elements = i;
		std::cout << std::to_string(i) << std::endl;

		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, i * sizeof(Util::vertex), vertices, GL_STATIC_DRAW);
		delete[] vertices;
	}

	void Chunk::render(GLint coord)
	{
		if(!elements)
		{
			return;
		}
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(coord, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, elements);
	}

	bool Chunk::is_visible(int x, int y, int z, int xadj, int yadj, int zadj)
	{
		//If the block does not exist/is "air" then it isn't visible.
		if(!voxel[x][y][z].type)
		{
			return false;
		}

		//If a block exists in the next slot along, then the face of this block is not visible, and we can skip it.
		if(get_block(xadj, yadj, zadj).type)
		{
			return false;
		}


		return true;

	}

	//This method is to be used with the possibility of the block being outside the chunk. 
	//If we can absolutely guarantee that it's not, simply use voxel[x][y][z].
	//It's almost certainly computationally cheaper.
	Block::Block Chunk::get_block(int x, int y, int z)
	{
		if(x < 0)
		{
			return l ? l->voxel[x + X][y][z] : 0;
		}
		if(x >= X)
		{
			return r ? r->voxel[x - X][y][z] : 0;
		}
		if(z < 0)
		{
			return f ? f->voxel[x][y][z + Z] : 0;
		}
		if(z >= Z)
		{
			return b ? b->voxel[x][y][z - Z] : 0;
		}
		return voxel[x][y][z];
	}

}
