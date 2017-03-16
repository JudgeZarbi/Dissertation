#include "threadsystem.h"

namespace Game
{
    ThreadSystem* ThreadSystem::instance = NULL;

	void ThreadSystem::initialise()
	{
		instance = new ThreadSystem();
	}

	void ThreadSystem::start_world_gen_threads(World* world)
	{
		wg_threads[0] = new WorldGenThread(0, 0, Game::CHUNKS_RANGE, Game::CHUNKS_RANGE, world, 0);
		wg_threads[0]->create_thread();
		wg_threads[1] = new WorldGenThread(0, -Game::CHUNKS_RANGE, Game::CHUNKS_RANGE, -1, world, 1);
		wg_threads[1]->create_thread();
		wg_threads[2] = new WorldGenThread(-Game::CHUNKS_RANGE, -Game::CHUNKS_RANGE, -1, -1, world, 2);
		wg_threads[2]->create_thread();
		wg_threads[3] = new WorldGenThread(-Game::CHUNKS_RANGE, 0, -1, Game::CHUNKS_RANGE, world, 3);
		wg_threads[3]->create_thread();		
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



