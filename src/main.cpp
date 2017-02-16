/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */
//Core libraries
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>
#include <AL/al.h>
#include <AL/alc.h>
#include <opus/opusfile.h>
#include <cstdio>

//Extra includes for my files.
#include "controls/keyboard.h"
#include "controls/mouse.h"
#include "texture/texture.h"
#include "util/global.h"
#include "util/physics.h"
#include "util/shader_utils.h"
#include "util/vertex.h"
#include "world/chunk.h"
#include "world/world.h"

int screen_width=800, screen_height=600;
GLuint program;
GLuint texarray;
GLint attribute_coord;
GLint uniform_mvp;
GLint uniform_texture;
GLuint cursor_vbo;
Game::World* world;

int mx, my, mz;
int face;
bool target;

//Audio stuff
ALuint source;
ALCdevice *dev;
ALCcontext *ctx;
OggOpusFile *f;
const int num_buffers = 5;
ALuint buffers[num_buffers];

int fill_buffer(ALuint buffer, OggOpusFile *file)
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

int update_stream()
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


bool init_resources()
{
	//wat even is this necessary for
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	world = new Game::World();

	texarray = Game::load_block_textures();

	program = Game::create_program("shaders/vert.glsl", "shaders/frag.glsl");

	if (!program)
	{
		return false;
	}
	
	attribute_coord = Game::get_attrib(program, "coord");
	uniform_mvp = Game::get_uniform(program, "mvp");
	uniform_texture = Game::get_uniform(program, "texarray");

	if (attribute_coord == -1 || uniform_mvp == -1 || uniform_texture == -1)
	{
		return false;		
	}	

	glPolygonOffset(1, 1);
	glEnableVertexAttribArray(attribute_coord);
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glUseProgram(program);
	glGenBuffers(1, &cursor_vbo);
	
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_texture, /*GL_TEXTURE*/0);
	
    // Open the default device.
    dev = alcOpenDevice(0);

    if (!dev)
    {
        fprintf(stderr, "Couldn't open OpenAL device.\n");
        return -1;
    }

    // We want an OpenAL context.
    ctx = alcCreateContext(dev, 0);

    alcMakeContextCurrent(ctx);

    if (!ctx)
    {
        fprintf(stderr,"Context fail\n");
        alcCloseDevice(dev);
        return -1;
    }

    // Get us a buffer and a source to attach it to.
    alGenSources(1, &source);

    // Set position and gain for the listener.
    alListener3f(AL_POSITION, 0.f,0.f,0.f);
    alListenerf(AL_GAIN,1.f);

    // ... and set source properties.
    alSource3f(source, AL_POSITION, 0.f,0.f,0.f);
    alSourcef(source, AL_GAIN, 1.0f);

    int error = 0;
    // Open the file.
    f = op_open_file("audio/music/FTF.opus", &error);
    if (error)
    {
        return error;
    }

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

	return true;
}

void render(SDL_Window* window)
{
	glm::mat4 view = glm::lookAt(Game::position, Game::position + Game::lookat, Game::up);
	glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 1000.0f);

	glm::mat4 mvp = projection * view;
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_POLYGON_OFFSET_FILL);

	world->render(attribute_coord, uniform_mvp, mvp);	
	
	/* At which voxel are we looking? */
	/* Very naive ray casting algorithm to find out which block we are looking at */

	glm::vec3 testpos = Game::position;
	glm::vec3 prevpos = Game::position;

	for(int i = 0; i < 100; i++) {
		/* Advance from our currect position to the direction we are looking at, in small steps */

		prevpos = testpos;
		testpos += Game::lookat * 0.1f;

		mx = floorf(testpos.x);
		my = floorf(testpos.y);
		mz = floorf(testpos.z);	

		/* If we find a block that is not air, we are done */

		if(world->get(mx, my, mz))
		{
			target = true;	
			break;		
		}
	}

	if(!world->get(mx, my, mz))
	{
		target = false;
	}

	glDisable(GL_POLYGON_OFFSET_FILL);
	glDisable(GL_CULL_FACE);
	glBindBuffer(GL_ARRAY_BUFFER, cursor_vbo);
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	if(target)
	{
		/* Find out which face of the block we are looking at */
		int px = floorf(prevpos.x);
		int py = floorf(prevpos.y);
		int pz = floorf(prevpos.z);

		if(px > mx)
			face = 0;
		else if(px < mx)
			face = 3;
		else if(py > my)
			face = 1;
		else if(py < my)
			face = 4;
		else if(pz > mz)
			face = 2;
		else if(pz < mz)
			face = 5;

		float bx = mx;
		float by = my;
		float bz = mz;

		/* Render a box around the block we are pointing at */

		float box[24][4] = {
			{bx + 0, by + 0, bz + 0, 8},
			{bx + 1, by + 0, bz + 0, 8},
			{bx + 0, by + 1, bz + 0, 8},
			{bx + 1, by + 1, bz + 0, 8},
			{bx + 0, by + 0, bz + 1, 8},
			{bx + 1, by + 0, bz + 1, 8},
			{bx + 0, by + 1, bz + 1, 8},
			{bx + 1, by + 1, bz + 1, 8},

			{bx + 0, by + 0, bz + 0, 8},
			{bx + 0, by + 1, bz + 0, 8},
			{bx + 1, by + 0, bz + 0, 8},
			{bx + 1, by + 1, bz + 0, 8},
			{bx + 0, by + 0, bz + 1, 8},
			{bx + 0, by + 1, bz + 1, 8},
			{bx + 1, by + 0, bz + 1, 8},
			{bx + 1, by + 1, bz + 1, 8},

			{bx + 0, by + 0, bz + 0, 8},
			{bx + 0, by + 0, bz + 1, 8},
			{bx + 1, by + 0, bz + 0, 8},
			{bx + 1, by + 0, bz + 1, 8},
			{bx + 0, by + 1, bz + 0, 8},
			{bx + 0, by + 1, bz + 1, 8},
			{bx + 1, by + 1, bz + 0, 8},
			{bx + 1, by + 1, bz + 1, 8},
		};

		glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_LINES, 0, 24);		
	}

	/* Draw a cross in the center of the screen */

	float cross[4][4] = {
		{-0.05, 0, 0, 9},
		{+0.05, 0, 0, 9},
		{0, -0.05, 0, 9},
		{0, +0.05, 0, 9},
	};

	glDisable(GL_DEPTH_TEST);
	glm::mat4 one(1);
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(one));
	glBufferData(GL_ARRAY_BUFFER, sizeof(cross), cross, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 4);

	/* And we are done */

	SDL_GL_SwapWindow(window);
}

void onResize(int width, int height)
{
	screen_width = width;
	screen_height = height;
	glViewport(0, 0, screen_width, screen_height);
}

void free_resources()
{
	glDeleteProgram(program);
	glDeleteTextures(1, &texarray);
}

void mainLoop(SDL_Window* window)
{
	static unsigned short keys;

    alSourcePlay(source);

	while (true) {
		SDL_Event ev;
		Game::timing();
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_KEYDOWN)
			{
				if(ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					return;
				}
				else
				{
					Game::keyDown(&ev.key, &keys);					
				}
			}
			else if (ev.type == SDL_KEYUP)
			{
				Game::keyUp(&ev.key, &keys);
			}
			else if (ev.type == SDL_MOUSEMOTION)
			{
				Game::mouseMotion(&ev.motion, &Game::angle);
				Game::update_vectors();
			}
			else if (ev.type == SDL_MOUSEBUTTONDOWN)
			{
				if (ev.button.button == SDL_BUTTON_RIGHT)
				{
					if(target)
					{
						if(world->get(mx, my, mz)->rightClick())
						{
							world->get_chunk(mx, my, mz)->changed = true;
						}
					}
				}
			}
			else if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				onResize(ev.window.data1, ev.window.data2);
			}
			else if (ev.type == SDL_QUIT)
			{
				return;
			}
		}
		if (keys)
		{
			Game::movement(keys);			
		}
//		std::cout << "(" << Game::position.x << ", " << Game::position.y << ", " << Game::position.z << ")" << std::endl;
		world->move();
		render(window);
		update_stream();
	}
}

int main() {
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window* window = SDL_CreateWindow("Many chunks and controls!",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		screen_width, screen_height,
		SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
	if (window == NULL)
	{
		std::cerr << "Error: can't create window: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	if (SDL_GL_CreateContext(window) == NULL)
	{
		std::cerr << "Error: SDL_GL_CreateContext: " << SDL_GetError() << std::endl;
		return EXIT_FAILURE;
	}

	glewExperimental = GL_TRUE;
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		std::cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << std::endl;
		return EXIT_FAILURE;
	}
	if (!GLEW_VERSION_3_3)
	{
		std::cerr << "Error: your graphic card does not support OpenGL 3.3" << std::endl;
		return EXIT_FAILURE;
	}

	// Set relative mouse mode, this will grab the cursor.
	SDL_SetRelativeMouseMode(SDL_TRUE);

	if (!init_resources())
	{
		return EXIT_FAILURE;
	}

	Game::position = glm::vec3(1, 150, 1);
	Game::angle = glm::vec3(0, -0.5, 0);
	Game::update_vectors();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    mainLoop(window);

	free_resources();
	return EXIT_SUCCESS;
}
