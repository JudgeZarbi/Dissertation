#include "world.h"

namespace Game
{
	World::World()
	{
		int offset = CHUNKS_RANGE;
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

		x_max = z_max = CHUNKS_RANGE;
		x_mpos = z_mpos = CHUNKS_X - 1;

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
					chunks[x][y][z]->build_vertices();
				}
			}
		}
	}

	void World::render(GLint coord, GLint uniform, glm::mat4 &pv)
	{
		int rendered = 0;
		for(int x = 0; x < CHUNKS_X; x++)
		{
			for(int y = 0; y < CHUNKS_Y; y++)
			{
				for(int z = 0; z < CHUNKS_Z; z++)
				{
					glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(chunks[x][y][z]->cx * X, chunks[x][y][z]->cy * Y, chunks[x][y][z]->cz * Z));
					glm::mat4 mvp = pv * model;

					// Is this chunk on the screen?
					glm::vec4 center = mvp * glm::vec4(chunks[x][y][z]->cx / 2, chunks[x][y][z]->cx / 2, chunks[x][y][z]->cx / 2, 1);

					center.x /= center.w;
					center.y /= center.w;

					// If it is behind the camera, don't bother drawing it
					if(center.z < -Y / 2)
						continue;

					// If it is outside the screen, don't bother drawing it
					if(fabsf(center.x) > 1 + fabsf(Y * 2 / center.w) || fabsf(center.y) > 1 + fabsf(Y * 2 / center.w))
						continue;

					glUniformMatrix4fv(uniform, 1, GL_FALSE, glm::value_ptr(mvp));

					chunks[x][y][z]->render(coord);
					rendered += 1;
				}
			}
		}
//		std::cout << rendered << std::endl;
	}	

	Block* World::get(int x, int y, int z) const
	{
		int cx = (((x >= 0 ? x : x-15) / X) + CHUNKS_RANGE) % CHUNKS_X;
		int cy = (y >= 0 ? y : y-15) / Y;
		int cz = (((z >= 0 ? z : z-15) / Z) + CHUNKS_RANGE) % CHUNKS_X;

		if(cx < 0 || cx >= CHUNKS_X || cy < 0 || cy >= CHUNKS_Y || cz < 0 || cz >= CHUNKS_Z)
			return 0;

		return chunks[cx][cy][cz]->get_block(x & (X - 1), y & (Y - 1), z & (Z - 1));
	}

	Chunk* World::get_chunk(int x, int y, int z) const
	{
		std::cout << (((x >= 0 ? x : x-15) / X) + CHUNKS_RANGE) % CHUNKS_X << " " << (y >= 0 ? y : y-15) / Y << " " << ((z >= 0 ? z : z-15) / Z) % CHUNKS_Z + CHUNKS_RANGE << std::endl;
		return chunks[(((x >= 0 ? x : x-15) / X) + CHUNKS_RANGE) % CHUNKS_X][(y >= 0 ? y : y-15) / Y][(((z >= 0 ? z : z-15) / Z) + CHUNKS_RANGE) % CHUNKS_Z];
	}

	void World::move()
	{
		std::cout << floor(position.x) << ", " << floor(position.y) << ", " << floor(position.z) << std::endl;
		Chunk* chunk = get_chunk(floor(position.x), floor(position.y), floor(position.z));
		glm::ivec3 cur_chunk = glm::ivec3(chunk->cx, chunk->cy, chunk->cz);

		int x = cur_chunk.x - x_old;
		int z = cur_chunk.z - z_old;

		std::cout << cur_chunk.x << " " << x_old << " " << x << std::endl;
		std::cout << cur_chunk.z << " " << z_old << " " << z << std::endl;
		std::cout << x_max << " " << z_max << std::endl;

		x_old = cur_chunk.x;
		z_old = cur_chunk.z;

		if (x > 0)
		{
			while (x != 0)
			{
				x_max++;
				x_mpos = (x_mpos + 1) % 11;
				for(int z = 0; z < CHUNKS_Z; z++)
				{
					int new_z = chunks[x_mpos][0][z]->cz;
					delete chunks[x_mpos][0][z];
					chunks[x_mpos][0][z] = new Chunk(x_max, 0, new_z);
					chunks[x_mpos][0][z]->initialise();
					chunks[x_mpos][0][z]->build_vertices();
				}
				x--;		
			}
		}
		else if (x < 0)
		{
			while (x != 0)
			{
				x_max--;
				x_mpos = (x_mpos - 1 + 11) % 11;
				for(int z = 0; z < CHUNKS_Z; z++)
				{
					int mark = (x_mpos + 1) % 11;
					int new_z = chunks[mark][0][z]->cz;
					delete chunks[mark][0][z];
					chunks[mark][0][z] = new Chunk(x_max - 10, 0, new_z);
					chunks[mark][0][z]->initialise();
					chunks[mark][0][z]->build_vertices();
				}
				x++;
			}
		}
		if (z > 0)
		{
			while (z != 0)
			{
				z_max++;
				z_mpos = (z_mpos + 1) % 11;
				for(int x = 0; x < CHUNKS_X; x++)
				{
					int new_x = chunks[x][0][z_mpos]->cx;
					delete chunks[x][0][z_mpos];
					chunks[x][0][z_mpos] = new Chunk(new_x, 0, z_max);
					chunks[x][0][z_mpos]->initialise();
					chunks[x][0][z_mpos]->build_vertices();
				}
				z--;
			}
		}
		else if (z < 0)
		{
			while (z != 0)
			{
				z_max--;
				z_mpos = (z_mpos - 1 + 11) % 11;
				for(int x = 0; x < CHUNKS_X; x++)
				{
					int mark = (z_mpos + 1) % 11;
					int new_x = chunks[x][0][mark]->cx;
					delete chunks[x][0][mark];
					chunks[x][0][mark] = new Chunk(new_x, 0, z_max - 10);
					chunks[x][0][mark]->initialise();
					chunks[x][0][mark]->build_vertices();
				}
				z++;
			}
		}

	}

}
