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
			if(wg_cur != wg_end)
			{
				wg_cur = (wg_cur + 1) % MAX_TASKS;
				if(world->chunks[wg_tasks[wg_cur].arr_x][0][wg_tasks[wg_cur].arr_z] != 0)
				{
//					delete world->chunks[wg_tasks[wg_cur].arr_x][0][wg_tasks[wg_cur].arr_z];
					world->chunks[wg_tasks[wg_cur].arr_x][0][wg_tasks[wg_cur].arr_z]->cx = wg_tasks[wg_cur].x;
					world->chunks[wg_tasks[wg_cur].arr_x][0][wg_tasks[wg_cur].arr_z]->cz = wg_tasks[wg_cur].z;
					world->chunks[wg_tasks[wg_cur].arr_x][0][wg_tasks[wg_cur].arr_z]->initialise();
					if (wg_tasks[wg_cur].arr_x == 15 && wg_tasks[wg_cur].arr_z == 6)
					{
//						std::cout << world->chunks[15][0][6]->cx << world->chunks[15][0][6]->cz << std::endl;
					}
				}
				else
				{				
					world->chunks[wg_tasks[wg_cur].arr_x][0][wg_tasks[wg_cur].arr_z] = new Chunk(wg_tasks[wg_cur].x, 0, wg_tasks[wg_cur].z);
					world->chunks[wg_tasks[wg_cur].arr_x][0][wg_tasks[wg_cur].arr_z]->initialise();
				}
//				std::cout << "Created chunk (" << wg_tasks[wg_cur].x << ", " << wg_tasks[wg_cur].z << ") at array (" << wg_tasks[wg_cur].arr_x << ", " << wg_tasks[wg_cur].arr_z << ")" << std::endl;
			}
			else if (r_cur != r_end)
			{
				r_cur = (r_cur + 1) % MAX_TASKS;
				world->chunks[r_tasks[r_cur].arr_x][0][r_tasks[r_cur].arr_z]->build_vertices();
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

	bool WorldGenThread::add_wg_task(int x, int z, int arr_x, int arr_z)
	{
		//No space left in the queue if this is true.
		if(wg_end == (wg_cur - 1 + MAX_TASKS) % MAX_TASKS)
		{
			std::cout << "No space!" << std::endl;
			return false;
		}
		else
		{
			wg_tasks[(wg_end + 1) % MAX_TASKS] = Task(x, z, arr_x, arr_z);
			wg_end = (wg_end + 1) % MAX_TASKS;
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

	bool WorldGenThread::add_r_task(int arr_x, int arr_z)
	{
		//No space left in the queue if this is true.
		if(r_end == (r_cur - 1 + MAX_TASKS) % MAX_TASKS)
		{
			std::cout << "No space!" << std::endl;
			return false;
		}
		else
		{
			r_tasks[(r_end + 1) % MAX_TASKS] = Task(0, 0, arr_x, arr_z);
			r_end = (r_end + 1) % MAX_TASKS;
			return true;
		}		
	}
}