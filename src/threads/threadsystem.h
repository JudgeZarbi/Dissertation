#ifndef THREADS_THREADSYSTEM_H
#define THREADS_THREADSYSTEM_H
#include "worldgenthread.h"
#include "audiothread.h"
#include "../audio/audiosystem.h"
#include "../world/world.h"

namespace Game
{
	struct ThreadSystem
	{
		static void initialise();

		virtual ~ThreadSystem();

		void start_audio_thread(AudioSystem* as);
		void start_world_gen_threads(World* world);
		WorldGenThread* get_world_gen_thread(int index);
		void stop_audio_thread();

		WorldGenThread* wg_threads[NUM_THREADS];
		AudioThread* a_thread;

		static ThreadSystem* instance;

	};

}

#endif