#include "worldgenthread.h"

namespace Game
{
	WorldGenThread::WorldGenThread(int x1, int z1, int x2, int z2, World* world) : world(world)
	{
		for(int x = x1; x <= x2; x++)
		{
			for(int z = z1; z <= z2; z++)
			{
				tasks[(end + 1) % MAX_TASKS] = Task(x, z, x + CHUNKS_RANGE, z + CHUNKS_RANGE);
				end = (end + 1) % MAX_TASKS;
				std::cout << "Task: x: " << tasks[end].x << ", z: " << tasks[end].z << ", x_arr: " << tasks[end].arr_x << ", z_arr: " << tasks[end].arr_z << std::endl;
			}
		}
	}

	void WorldGenThread::create_thread()
	{
		thread = new std::thread(&WorldGenThread::loop, this);
	}

	void WorldGenThread::loop()
	{
		//Should probably add a kill signal so I can exit more cleanly than I do.
		while(true)
		{
			if(cur != end)
			{
				cur = (cur + 1) % MAX_TASKS;
//				if(world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z] != 0)
//				{
//					delete world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z];
//				}
				world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z] = new Chunk(tasks[cur].x, 0, tasks[cur].z);
				world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z]->initialise();
				world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z]->build_vertices();
			}
			else
			{
				usleep(500*1000);
				busy = false;
			}
		}
	}

	bool WorldGenThread::add_task(int x, int z, int arr_x, int arr_z)
	{
		//No space left in the queue if this is true.
		if(end == (cur + 1) % MAX_TASKS)
		{
			return false;
		}
		else
		{
			tasks[end + 1 % MAX_TASKS] = Task(x, z, arr_x, arr_z);
			end = end + 1 % MAX_TASKS;
			return true;
		}
	}
}