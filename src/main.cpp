/**
 * Based on code at https://gitlab.com/wikibooks-opengl/modern-tutorials/tree/master/glescraft-sdl2
 * which is in the public domain.
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
#include <thread>
#include <unistd.h>
#include <chrono>

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
#include "audio/audiosystem.h"
#include "threads/worldgenthread.h"
#include "render/renderer.h"
#include "threads/audiothread.h"
#include "threads/threadsystem.h"

Game::World* world;
Game::AudioSystem* as;
Game::Renderer* r;
Game::ThreadSystem* ts;

int mx, my, mz;
int face;
bool target;

bool init_resources()
{
	world = new Game::World();

	Game::AudioSystem::initialise();
	as = Game::AudioSystem::instance;

	Game::ThreadSystem::initialise();
	ts = Game::ThreadSystem::instance;
	ts->start_world_gen_threads(world);
	
	r = new Game::Renderer();
	r->initialise();

	return true;
}

void render()
{
	r->render(world);
}

void free_resources()
{
	//Also deletes the audiosystem
	delete ts;
	delete world;
	delete r;
}

void pause_loop(unsigned short keys)
{
	while(true)
	{
		SDL_Event ev;
		Game::timing();
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_KEYDOWN)
			{
				if(ev.key.keysym.scancode == SDL_SCANCODE_P)
				{
					SDL_SetRelativeMouseMode(SDL_TRUE);				
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
			else if (ev.type == SDL_WINDOWEVENT && ev.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
			{
				r->onResize(ev.window.data1, ev.window.data2);
			}
			else if (ev.type == SDL_QUIT)
			{
				return;
			}
		}
	}
}

void mainLoop()
{
	static unsigned short keys;

	as->play_sound("FTF");
	ts->start_audio_thread(as);

	double max = 999.0;
	double min;
	double total = 0;
	int num = 0;

	while (true) {
	    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	    start = std::chrono::high_resolution_clock::now();
		SDL_Event ev;
		Game::timing();
		while (SDL_PollEvent(&ev))
		{
			if (ev.type == SDL_KEYDOWN)
			{
				if(ev.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
				{
					std::cout << "Max FPS: " << std::to_string(1/max) << std::endl;
					std::cout << "Min FPS: " << std::to_string(1/min) << std::endl;
					std::cout << "Avg FPS: " << std::to_string(1/(total/num)) << std::endl;
					return;
				}
				else if(ev.key.keysym.scancode == SDL_SCANCODE_P)
				{
					SDL_SetRelativeMouseMode(SDL_FALSE);
					pause_loop(keys);
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
				r->onResize(ev.window.data1, ev.window.data2);
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
		Game::gravity(world);
//		std::cout << "(" << Game::position.x << ", " << Game::position.y << ", " << Game::position.z << ")" << std::endl;
		world->move(ts->wg_threads);
		render();
	    end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> dur_d = end-start;
		double dur = dur_d.count();
	    if (dur < max && num > 0)
	    {
	    	max = dur;
	    }
	    else if (dur > min && num > 0)
	   	{
	   		min = dur;
	   	}
	   	total += dur;
	   	std::cout << std::to_string(dur) << std::endl;
	   	num++;
	}
}

int main() {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();
	if (!init_resources())
	{
		return EXIT_FAILURE;
	}

	Game::position = glm::vec3(1, 150, 1);
	Game::angle = glm::vec3(0, -0.5, 0);
	Game::update_vectors();

    for (int i = 0; i < Game::NUM_THREADS; i++)
    {
    	while(ts->wg_threads[i]->busy)
    	{
#ifdef __linux__
				usleep(10*1000);
#elif _WIN32
				Sleep(10);
#endif
    	}
    }

//    world->consistency();

    end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "Time to start: " << elapsed_seconds.count() << std::endl;
    mainLoop();

	free_resources();
	return EXIT_SUCCESS;
}
