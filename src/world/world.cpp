#include "world.h"

namespace World
{
	World::World()
	{
		for(int x = 0; x < CHUNKS_X; x++)
		{
			for(int y = 0; y < CHUNKS_Y; y++)
			{
				for(int z = 0; z < CHUNKS_Z; z++)
				{
					chunks[x][y][z] = new Chunk();
					chunks[x][y][z]->initialise(x, y, z);
				}
			}
		}

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
					glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x * X, y * Y, z * Z));
					glm::mat4 mvp = pv * model;

					// Is this chunk on the screen?
					glm::vec4 center = mvp * glm::vec4(X / 2, Y / 2, Z / 2, 1);

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
		std::cout << rendered << std::endl;
	}	
}
