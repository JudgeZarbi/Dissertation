#include "worldgenthread.h"

namespace Game
{
	WorldGenThread::WorldGenThread(int x1, int z1, int x2, int z2, World* world, int id) : world(world), id(id)
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
				std::cout << id << ": " << std::to_string(cur) << " " << std::to_string(end) << std::endl;
				std::cout << "Created chunk at (" << tasks[cur].x << ", " << tasks[cur].z << ")" << std::endl;
				std::cout << "Created chunk at array (" << tasks[cur].arr_x << ", " << tasks[cur].arr_z << ")" << std::endl;				
			}
			else
			{
				busy = false;
				usleep(500*1000);
			}
		}
	}

	bool WorldGenThread::add_task(int x, int z, int arr_x, int arr_z)
	{
		//No space left in the queue if this is true.
		if(end == (cur - 1 + MAX_TASKS) % MAX_TASKS)
		{
			std::cout << "No space!" << std::endl;
			return false;
		}
		else
		{
			tasks[(end + 1) % MAX_TASKS] = Task(x, z, arr_x, arr_z);
			end = (end + 1) % MAX_TASKS;
			std::cout << "Task added: " << id << " - " << std::to_string(end) << " " << std::to_string(x) << " " << std::to_string(z) << std::endl;
			return true;
		}
	}
}