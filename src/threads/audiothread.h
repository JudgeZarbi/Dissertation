#ifndef THREADS_AUDIOTHREAD_H
#define THREADS_AUDIOTHREAD_H

#include <thread>
#include "../audio/audiosystem.h"

namespace Game
{
	struct AudioThread
	{
		AudioThread(AudioSystem* sys);
		virtual ~AudioThread();

		void create_thread();
		void loop();
		void stop();

		std::thread* thread;
		AudioSystem* system;
		bool run = true;
	};
}

#endif