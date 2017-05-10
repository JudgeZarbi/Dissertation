#ifndef AUDIO_AUDIOSYSTEM_H
#define AUDIO_AUDIOSYSTEM_H

#include <AL/al.h>
#include <AL/alc.h>
#include <cstdio>
#include "sound/sound.h"
#include <unordered_map>
#include <vector>
#include <unistd.h>

namespace Game
{
	struct AudioSystem
	{
		AudioSystem();
		virtual ~AudioSystem();

		static void initialise();
		void play_sound(std::string name);
		void update_sounds();

		ALCdevice *dev;
		ALCcontext *ctx;
		static AudioSystem* instance;

		std::vector<Sound*> sounds;

		std::unordered_map<std::string, std::string> sound_map;
	};
}

#endif