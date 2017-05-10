#ifndef AUDIO_SOUND_SOUND_H
#define AUDIO_SOUND_SOUND_H

#include <opus/opusfile.h>
#include <AL/al.h>
#include <AL/alc.h>

#include <iostream>

namespace Game
{
	const int NUM_BUFFERS = 5;
    const int BUFFER_SIZE = 960*32*2;

	struct Sound
	{
	    ALuint source;
	    ALuint buffers[NUM_BUFFERS];
	    OggOpusFile *f;

		Sound();
		virtual ~Sound();

		int fill_buffer(ALuint buffer, OggOpusFile* file);
		int update_stream();
		void play();
	};	
}


#endif