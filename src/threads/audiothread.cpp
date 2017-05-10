#include "audiothread.h"

namespace Game
{
	AudioThread::AudioThread(AudioSystem* sys)
	{
		system = sys->instance;
	}

	AudioThread::~AudioThread()
	{
		run = false;
		thread->join();
		delete thread;		
	}

	void AudioThread::create_thread()
	{
		thread = new std::thread(&AudioThread::loop, this);
	}

	void AudioThread::loop()
	{
		while(run)
		{
			system->update_sounds();
		}
		delete system;
	}

	void AudioThread::stop()
	{
	}
}