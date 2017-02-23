#ifndef AUDIO_AUDIOSYSTEM_H
#define AUDIO_AUDIOSYSTEM_H

#include <AL/al.h>
#include <AL/alc.h>
#include <cstdio>
#include "sound/sound.h"

namespace Game
{
	struct AudioSystem
	{
		ALCdevice *dev;
		ALCcontext *ctx;
		static AudioSystem* instance;
		Sound* sound;

		AudioSystem();

		static void initialise();
		void play();
		void update();
	};
}

#endif