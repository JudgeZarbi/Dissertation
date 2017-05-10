#ifndef _THREADS_WORLDGENTHREAD_H
#define _THREADS_WORLDGENTHREAD_H

#include <thread>
#include <vector>
#include <string>
#include <unistd.h>
#include "../world/world.h"
#include "../util/global.h"

namespace Game
{
	struct World;

	struct Task
	{
		int x;
		int z;
		int arr_x;
		int arr_z;

		Task(): x(0), z(0), arr_x(0), arr_z(0) {};
		Task(int x, int z, int arr_x, int arr_z): x(x), z(z), arr_x(arr_x), arr_z(arr_z) {};
	};

	struct WorldGenThread
	{
		WorldGenThread(World* world);
		virtual ~WorldGenThread();

		void create_thread();
		void loop();
		bool add_wg_task(int x, int z, int arr_x, int arr_z);
		bool add_r_task(int arr_x, int arr_z);

		Task wg_tasks[MAX_TASKS];
		Task r_tasks[MAX_TASKS];
		std::thread* thread;
		uint16_t wg_cur = MAX_TASKS - 1;
		uint16_t wg_end = MAX_TASKS - 1;
		uint16_t r_cur = MAX_TASKS - 1;
		uint16_t r_end = MAX_TASKS - 1;
		World* world;
		bool busy = true;
		bool done = false;
		int id;
		bool run = true;
	};
}

#endif