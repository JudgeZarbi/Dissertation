#include "worldgenthread.h"

namespace Game
{
	WorldGenThread::WorldGenThread(World* world) : world(world)
	{

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
				if(world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z] != 0)
				{
//					delete world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z];
					world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z]->cx = tasks[cur].x;
					world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z]->cz = tasks[cur].z;
					world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z]->initialise();
					world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z]->build_vertices();
					if (tasks[cur].arr_x == 15 && tasks[cur].arr_z == 6)
					{
//						std::cout << world->chunks[15][0][6]->cx << world->chunks[15][0][6]->cz << std::endl;
					}
				}
				else
				{				
					world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z] = new Chunk(tasks[cur].x, 0, tasks[cur].z);
					world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z]->initialise();
					world->chunks[tasks[cur].arr_x][0][tasks[cur].arr_z]->build_vertices();
				}
//				std::cout << "Created chunk (" << tasks[cur].x << ", " << tasks[cur].z << ") at array (" << tasks[cur].arr_x << ", " << tasks[cur].arr_z << ")" << std::endl;
			}
			else
			{
				if(done)
				{
					busy = false;				
				}
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
			if (arr_x == 15 && arr_z == 6)
			{
//				std::cout << "Target marked for destruction!" << " " << x << " " << z << std::endl;
			}
			if (arr_x == 16 && arr_z == 6)
			{
//				std::cout << "Target adjacent!" << " " << x << " " << z << std::endl;
			}
//			std::cout << "Task added: " << std::to_string(x) << " " << std::to_string(z) << std::endl;
			return true;
		}
	}
}