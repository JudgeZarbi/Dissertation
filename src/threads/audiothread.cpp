#include "audiothread.h"

namespace Game
{
	AudioThread::AudioThread(AudioSystem* sys)
	{
		system = sys->instance;
	}

	void AudioThread::create_thread()
	{
		thread = new std::thread(&AudioThread::loop, this);
	}

	void AudioThread::loop()
	{
		while(true)
		{
			system->update_sounds();
		}
	}
}