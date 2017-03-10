#ifndef THREADS_AUDIOTHREAD_H
#define THREADS_AUDIOTHREAD_H

#include <thread>
#include "../audio/audiosystem.h"

namespace Game
{
	struct AudioThread
	{
		AudioThread(AudioSystem* sys);

		void create_thread();
		void loop();

		std::thread* thread;
		AudioSystem* system;
	};
}

#endif