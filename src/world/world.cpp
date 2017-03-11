#include "world.h"

namespace Game
{
	World::World()
	{
		memset(chunks, 0, sizeof(chunks));
		x_max = z_max = CHUNKS_RANGE;
		x_mpos = z_mpos = CHUNKS_X - 1;
/*		int offset = CHUNKS_RANGE;
		for(int x = 0; x < CHUNKS_X; x++)
		{
			for(int y = 0; y < CHUNKS_Y; y++)
			{
				for(int z = 0; z < CHUNKS_Z; z++)
				{
					chunks[x][y][z] = new Chunk(x - offset, y, z - offset);
					chunks[x][y][z]->initialise();
					std::cout << chunks[x][y][z]->cx << ", " << chunks[x][y][z]->cy << ", " << chunks[x][y][z]->cz << std::endl;
					if (chunks[x][y][z]->cx == 1 && chunks[x][y][z]->cz == 1)
					{
						chunks[x][y][z]->voxel[8][148][3] = new ColourBlock();
					}
				}
			}
		}

*/
	}

	Block* World::get(int x, int y, int z) const
	{
		int cx = (((((x >= 0 ? x : x-15) / X) + CHUNKS_RANGE) % CHUNKS_X) + CHUNKS_X) % CHUNKS_X;
		int cy = (y >= 0 ? y : y-15) / Y;
		int cz = (((((z >= 0 ? z : z-15) / Z) + CHUNKS_RANGE) % CHUNKS_Z) + CHUNKS_Z) % CHUNKS_Z;

		if(cx < 0 || cx >= CHUNKS_X || cy < 0 || cy >= CHUNKS_Y || cz < 0 || cz >= CHUNKS_Z)
			return 0;

		return chunks[cx][cy][cz]->get_block(x & (X - 1), y & (Y - 1), z & (Z - 1));
	}

	Chunk* World::get_chunk(int x, int y, int z) const
	{
		std::cout << ((x >= 0 ? x : x-15) / X) << " " << ((z >= 0 ? z : z-15) / Z) << std::endl;
//		std::cout << (((((x >= 0 ? x : x-15) / X) + CHUNKS_RANGE) % CHUNKS_X) + CHUNKS_X) % CHUNKS_X << " " << (((((z >= 0 ? z : z-15) / Z) + CHUNKS_RANGE) % CHUNKS_Z) + CHUNKS_Z) % CHUNKS_Z << std::endl;
//		std::cout << chunks[15][0][6]->cx << chunks[15][0][6]->cz << std::endl;
		return chunks[(((((x >= 0 ? x : x-15) / X) + CHUNKS_RANGE) % CHUNKS_X) + CHUNKS_X) % CHUNKS_X][0][(((((z >= 0 ? z : z-15) / Z) + CHUNKS_RANGE) % CHUNKS_Z) + CHUNKS_Z) % CHUNKS_Z];
	}

	void World::move(WorldGenThread** wg_threads)
	{
//		std::cout << position.x << ", " << position.y << ", " << position.z << std::endl;
//		std::cout << floor(position.x) << ", " << floor(position.y) << ", " << floor(position.z) << std::endl;
		Chunk* chunk = get_chunk(floor(position.x), floor(position.y), floor(position.z));
		glm::ivec3 cur_chunk = glm::ivec3(chunk->cx, chunk->cy, chunk->cz);

		int x = cur_chunk.x - x_old;
		int z = cur_chunk.z - z_old;

//		std::cout << "x: " <<cur_chunk.x << " " << x_old << " " << x << std::endl;
//		std::cout << "z: " << cur_chunk.z << " " << z_old << " " << z << std::endl;

		if (x > 0)
		{
			while (x != 0)
			{
				x_max++;
				x_mpos = (x_mpos + 1) % CHUNKS_X;
				for(int z = 0; z < CHUNKS_Z; z++)
				{
					int new_z = chunks[x_mpos][0][z]->cz;
//					std::cout << "Deleting " << chunks[x_mpos][0][z] << std::endl;
//					std::cout << "Deleted chunk at (" << chunks[x_mpos][0][z]->cx << ", " << chunks[x_mpos][0][z]->cz << ")" << std::endl;
//					delete chunks[x_mpos][0][z];
					wg_threads[(x_mpos + z) % 4]->add_task(x_max, new_z, x_mpos, z);
				}
				x--;		
			}
		}
		else if (x < 0)
		{
			while (x != 0)
			{
				x_max--;
				x_mpos = (x_mpos - 1 + CHUNKS_X) % CHUNKS_X;
				for(int z = 0; z < CHUNKS_Z; z++)
				{
					int mark = (x_mpos + 1) % CHUNKS_X;
					int new_z = chunks[mark][0][z]->cz;
//					std::cout << "Deleting " << chunks[mark][0][z] << std::endl;
//					std::cout << "Deleted chunk at (" << chunks[mark][0][z]->cx << ", " << chunks[mark][0][z]->cz << ")" << std::endl;
//					delete chunks[mark][0][z];
					wg_threads[(mark + z) % 4]->add_task(x_max - DIFF, new_z, mark, z);
				}
				x++;
			}
		}
		if (z > 0)
		{
			while (z != 0)
			{
				z_max++;
				z_mpos = (z_mpos + 1) % CHUNKS_Z;
				for(int x = 0; x < CHUNKS_X; x++)
				{
					int new_x = chunks[x][0][z_mpos]->cx;
//					std::cout << "Deleting " << chunks[x][0][z_mpos] << std::endl;
//					std::cout << "Deleted chunk at (" << chunks[x][0][z_mpos]->cx << ", " << chunks[x][0][z_mpos]->cz << ")" << std::endl;
//					delete chunks[x][0][z_mpos];
					wg_threads[(x + z_mpos) % 4]->add_task(new_x, z_max, x, z_mpos);
				}
				z--;
			}
		}
		else if (z < 0)
		{
			while (z != 0)
			{
				z_max--;
				z_mpos = (z_mpos - 1 + CHUNKS_Z) % CHUNKS_Z;
				for(int x = 0; x < CHUNKS_X; x++)
				{
					int mark = (z_mpos + 1) % CHUNKS_Z;
					int new_x = chunks[x][0][mark]->cx;
//					std::cout << "Deleting " << chunks[x][0][mark] << std::endl;
//					std::cout << "Deleted chunk at (" << chunks[x][0][mark]->cx << ", " << chunks[x][0][mark]->cz << ")" << std::endl;
//					delete chunks[x][0][mark];
					wg_threads[(x + mark) % 4]->add_task(new_x, z_max - DIFF, x, mark);
				}
				z++;
			}
		}

		x_old = cur_chunk.x;
		z_old = cur_chunk.z;

	}

	void World::consistency()
	{
		for(int x = 0; x < CHUNKS_X; x++)
		{
			for(int y = 0; y < CHUNKS_Y; y++)
			{
				for(int z = 0; z < CHUNKS_Z; z++)
				{
					if(x > 0)
					{
						chunks[x][y][z]->l = chunks[x-1][y][z];
					}
					if(x < CHUNKS_X - 1)
					{
						chunks[x][y][z]->r = chunks[x+1][y][z];
					}
					if(y > 0)
					{
						chunks[x][y][z]->d = chunks[x][y-1][z];
					}
					if(y < CHUNKS_Y - 1)
					{
						chunks[x][y][z]->u = chunks[x][y+1][z];					
					}
					if(z > 0)
					{
						chunks[x][y][z]->f = chunks[x][y][z-1];
					}
					if(z < CHUNKS_Z - 1)
					{
						chunks[x][y][z]->b = chunks[x][y][z+1];
					}
				}
			}
		}
	}

}
