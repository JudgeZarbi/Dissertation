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
		WorldGenThread(int x1, int z1, int x2, int z2, World* world, int id);

		void create_thread();
		void loop();
		bool add_task(int x, int z, int arr_x, int arr_z);

		Task tasks[MAX_TASKS];
		std::thread* thread;
		uint8_t cur = MAX_TASKS - 1;
		uint8_t end = MAX_TASKS - 1;
		World* world;
		bool busy = true;
		int id;
	};
}

#endif