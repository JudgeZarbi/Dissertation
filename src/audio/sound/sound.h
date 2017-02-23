#ifndef AUDIO_SOUND_SOUND_H
#define AUDIO_SOUND_SOUND_H

#include <opus/opusfile.h>
#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>

namespace Game
{

const int num_buffers = 5;

	struct Sound
	{
	    ALuint source;
	    ALuint buffers[num_buffers];
	    OggOpusFile *f;

		Sound();

		int fill_buffer(ALuint buffer, OggOpusFile* file);
		int update_stream();
		void play();
	};	
}


#endif