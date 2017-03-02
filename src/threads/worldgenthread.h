#include <thread>
#include <vector>
#include <unistd.h>
#include "../world/world.h"

namespace Game
{
	const static int MAX_TASKS = 50;

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
		WorldGenThread(int x1, int z1, int x2, int z2, World* world);

		void create_thread();
		void loop();
		bool add_task(int x, int z, int arr_x, int arr_z);

		Task tasks[MAX_TASKS];
		std::thread* thread;
		uint8_t cur = 49;
		uint8_t end = 49;
		World* world;
		bool busy = true;
	};
}