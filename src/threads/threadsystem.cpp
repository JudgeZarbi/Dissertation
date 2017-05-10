#include "threadsystem.h"

namespace Game
{
    ThreadSystem* ThreadSystem::instance = NULL;

	void ThreadSystem::initialise()
	{
		instance = new ThreadSystem();
	}

	ThreadSystem::~ThreadSystem()
	{
		delete a_thread;
		for (int i = 0; i < NUM_THREADS; i++)
		{
			delete wg_threads[i];
		}
		ThreadSystem::instance = NULL;
	}

	void ThreadSystem::start_world_gen_threads(World* world)
	{
		for(int i = 0; i < NUM_THREADS; i++)
		{
			wg_threads[i] = new WorldGenThread(world);
			wg_threads[i]->create_thread();
		}

		for(int x = 0; x < CHUNKS_X; x++)
		{
			for(int z = 0; z < CHUNKS_Z; z++)
			{
				wg_threads[(x + z) % NUM_THREADS]->add_wg_task(x - CHUNKS_RANGE, z - CHUNKS_RANGE, x, z);
			}
		}

		for(int x = CHUNKS_X - CHUNKS_RANGE - FRAME_RANGE - VERT_RANGE; x <= CHUNKS_X - CHUNKS_RANGE + FRAME_RANGE + VERT_RANGE; x++)
		{
			for(int z = CHUNKS_Z - CHUNKS_RANGE - FRAME_RANGE - VERT_RANGE; z <= CHUNKS_Z - CHUNKS_RANGE + FRAME_RANGE + VERT_RANGE; z++)
			{
				wg_threads[(x + z) % NUM_THREADS]->add_r_task(x, z);
			}
		}

		for (int i = 0; i < NUM_THREADS; i++)
		{
			wg_threads[i]->done = true;
		}
	}

	void ThreadSystem::start_audio_thread(AudioSystem* as)
	{
		a_thread = new Game::AudioThread(as);
		a_thread->create_thread();
	}

	WorldGenThread* ThreadSystem::get_world_gen_thread(int index)
	{
		return wg_threads[index];
	}

}



