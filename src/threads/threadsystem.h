#ifndef THREADS_THREADSYSTEM_H
#define THREADS_THREADSYSTEM_H
#include "worldgenthread.h"
#include "audiothread.h"
#include "../audio/audiosystem.h"
#include "../world/world.h"

namespace Game
{
	const static int NUM_THREADS = 4;

	struct ThreadSystem
	{
		static void initialise();
		void start_audio_thread(AudioSystem* as);
		void start_world_gen_threads(World* world);
		WorldGenThread* get_world_gen_thread(int index);

		WorldGenThread* wg_threads[NUM_THREADS];
		AudioThread* a_thread;

		static ThreadSystem* instance;

	};

}

#endif