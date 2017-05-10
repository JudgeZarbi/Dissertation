/**
 * Based on code at https://davidgow.net/hacks/opusal.html
 * which is in the public domain.
 */

#include "sound.h"

namespace Game
{
    Sound::Sound()
    {    	
        // Get us a buffer and a source to attach it to.
        alGenSources(1, &source);

        // ... and set source properties.
        alSource3f(source, AL_POSITION, 0.f,0.f,0.f);
        alSourcef(source, AL_GAIN, 1.0f);

        // Open the file.
        f = op_open_file("audio/music/FTF.opus", NULL);

        // Get the number of channels in the current link.
        int num_channels = op_channel_count(f,-1);
        // Get the number of samples (per channel) in the current link.
        int pcm_size = op_pcm_total(f,-1);

        std::cout << "FTF.opus" << ": " << num_channels << " channels, " << pcm_size << " samples (" << pcm_size/48000 << " seconds)" << std::endl;

        alGenBuffers(NUM_BUFFERS,buffers);

        for (int cur_buf = 0; cur_buf < NUM_BUFFERS; ++cur_buf)
        {
            fill_buffer(buffers[cur_buf],f);        
        }

        alSourceQueueBuffers(source,NUM_BUFFERS,buffers);
    }

    Sound::~Sound()
    {
        alSourceUnqueueBuffers(source,NUM_BUFFERS,buffers);

        // We have to delete the source here, as OpenAL soft seems to need
        // the source gone before the buffers. Perhaps this is just timing.
        alDeleteSources(1,&source);
        alDeleteBuffers(NUM_BUFFERS,buffers);

        // Close the opus file.
        op_free(f);        
    }

    int Sound::fill_buffer(ALuint buffer, OggOpusFile* file)
    {
        // Let's have a buffer that is two opus frames long (and two channels)
        int16_t buf[BUFFER_SIZE];

        int samples_read = 0;

        int num_channels = op_channel_count(file, -1);

    //    printf("Filling buffer %d...\n",buffer);

        // We only support stereo and mono, set the openAL format based on channels.
        // opus always uses signed 16-bit integers, unless the _float functions are called.
        ALenum format;
        if (num_channels == 1)
        {
            format = AL_FORMAT_MONO16;
        }
        else if (num_channels == 2)
        {
            format = AL_FORMAT_STEREO16;
        }
        else
        {
            std::cerr << "File contained more channels than we support (" << num_channels << ")." << std::endl;
            return OP_EIMPL;
        }

        // Keep reading samples until we have them all.
        while (samples_read < BUFFER_SIZE)
        {
            // op_read returns number of samples read (per channel), and accepts number of samples which fit in the buffer, not number of bytes.
            int ns = op_read(file, buf + samples_read*num_channels, (BUFFER_SIZE-samples_read*num_channels), 0);
            if (ns < 0)
            {
                return ns;
            }
            if (ns == 0)
            {
                if (samples_read == 0)
                {
                    op_pcm_seek(file, 0);
                }
                else
                {
                    break;
                }
            } 
            samples_read += ns;
        }

        alBufferData(buffer, format, buf, samples_read*num_channels*2, 48000);

        return samples_read;
    }

    int Sound::update_stream()
    {
        int num_processed_buffers = 0;
        ALuint currentbuffer;

        // How many buffers do we need to fill?
        alGetSourcei(source, AL_BUFFERS_PROCESSED, &num_processed_buffers);

        ALenum source_state;
        alGetSourcei(source, AL_SOURCE_STATE, &source_state);
        if (source_state != AL_PLAYING)
        {
            std::cout << "Source not playing!" << std::endl;
            alSourcePlay(source);
        }

        // Unqueue a finished buffer, fill it with new data, and re-add it to the end of the queue.
        while (num_processed_buffers--)
        {
            alSourceUnqueueBuffers(source, 1 ,&currentbuffer);

            if (fill_buffer(currentbuffer, f) <= 0)
                return 0;

            alSourceQueueBuffers(source, 1, &currentbuffer);
        }

        return 1;
    }

    void Sound::play()
    {
        alSourcePlay(source);
    }
}