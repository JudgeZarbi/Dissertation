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

        int error = 0;
        // Open the file.
        f = op_open_file("audio/music/FTF.opus", &error);

        // Get the number of channels in the current link.
        int num_channels = op_channel_count(f,-1);
        // Get the number of samples (per channel) in the current link.
        int pcm_size = op_pcm_total(f,-1);

        printf("%s: %d channels, %d samples (%d seconds)\n","FTF.opus", num_channels, pcm_size, pcm_size/48000);

        alGenBuffers(num_buffers,buffers);

        for (int cur_buf = 0; cur_buf < num_buffers; ++cur_buf)
        {
            fill_buffer(buffers[cur_buf],f);        
        }

        alSourceQueueBuffers(source,num_buffers,buffers);
    }

    int Sound::fill_buffer(ALuint buffer, OggOpusFile* file)
    {
        // Let's have a buffer that is two opus frames long (and two channels)
        const int buffer_size = 960*32*2;
        int16_t buf[buffer_size];

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
            fprintf(stderr, "File contained more channels than we support (%d).\n", num_channels);
            return OP_EIMPL;
        }

        // Keep reading samples until we have them all.
        while (samples_read < buffer_size)
        {
            // op_read returns number of samples read (per channel), and accepts number of samples which fit in the buffer, not number of bytes.
            int ns = op_read(file, buf + samples_read*num_channels, (buffer_size-samples_read*num_channels), 0);
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
            printf("Source not playing!\n");
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