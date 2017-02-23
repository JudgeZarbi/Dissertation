#include "audiosystem.h"

namespace Game
{
    AudioSystem* AudioSystem::instance = NULL;

    void AudioSystem::initialise()
    {
        instance = new AudioSystem();
    }

    AudioSystem::AudioSystem()
    {
        // Open the default device.
        dev = alcOpenDevice(0);

        if (!dev)
        {
            fprintf(stderr, "Couldn't open OpenAL device.\n");
        }

        // We want an OpenAL context.
        ctx = alcCreateContext(dev, 0);

        alcMakeContextCurrent(ctx);

        if (!ctx)
        {
            fprintf(stderr,"Context fail\n");
            alcCloseDevice(dev);
        }

        // MOVE THIS ELSEWHERE LATER. IT'S FINE HERE FOR NOW.
        // Set position and gain for the listener.
        alListener3f(AL_POSITION, 0.f,0.f,0.f);
        alListenerf(AL_GAIN,1.f);

        sound = new Sound();

    }

    void AudioSystem::play()
    {
        sound->play();
    }

    void AudioSystem::update_loop()
    {
        while(true)
        {
            sound->update_stream();            
        }
    }

}
