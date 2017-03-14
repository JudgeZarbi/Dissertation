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
            std::cerr << "Couldn't open OpenAL device." << std::endl;
        }

        // We want an OpenAL context.
        ctx = alcCreateContext(dev, 0);

        alcMakeContextCurrent(ctx);

        if (!ctx)
        {
            std::cerr << "Context fail" << std::endl;
            alcCloseDevice(dev);
        }

        // MOVE THIS ELSEWHERE LATER. IT'S FINE HERE FOR NOW.
        // Set position and gain for the listener.
        alListener3f(AL_POSITION, 0.f,0.f,0.f);
        alListenerf(AL_GAIN,1.f);

    }

    void AudioSystem::play_sound(std::string name)
    {
        sounds.push_back(new Sound());
        sounds[0]->play();
    }

    void AudioSystem::update_sounds()
    {
        while(true)
        {
            sounds[0]->update_stream();
            usleep(50*1000);          
        }
    }

}
