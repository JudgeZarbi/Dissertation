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
#include <thread>
#include <unistd.h>

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

Game::World* world;
Game::AudioSystem* as;
Game::WorldGenThread* wg_threads[4];
Game::Renderer* r;

int mx, my, mz;
int face;
bool target;

bool init_resources()
{
	world = new Game::World();

	wg_threads[0] = new Game::WorldGenThread(0, 0, Game::CHUNKS_RANGE, Game::CHUNKS_RANGE, world);
	wg_threads[0]->create_thread();
	wg_threads[1] = new Game::WorldGenThread(0, -Game::CHUNKS_RANGE, Game::CHUNKS_RANGE, -1, world);
	wg_threads[1]->create_thread();
	wg_threads[2] = new Game::WorldGenThread(-Game::CHUNKS_RANGE, -Game::CHUNKS_RANGE, -1, -1, world);
	wg_threads[2]->create_thread();
	wg_threads[3] = new Game::WorldGenThread(-Game::CHUNKS_RANGE, 0, -1, Game::CHUNKS_RANGE, world);
	wg_threads[3]->create_thread();

	Game::AudioSystem::initialise();
	as = Game::AudioSystem::instance;
	
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
	std::thread sound(&Game::AudioSystem::update_sounds, as);

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
				else if(ev.key.keysym.scancode == SDL_SCANCODE_P)
				{
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
//		std::cout << "(" << Game::position.x << ", " << Game::position.y << ", " << Game::position.z << ")" << std::endl;
		world->move();
		render();
	}
}

int main() {
	if (!init_resources())
	{
		return EXIT_FAILURE;
	}

	Game::position = glm::vec3(1, 150, 1);
	Game::angle = glm::vec3(0, -0.5, 0);
	Game::update_vectors();

    for (int i = 0; i < 4; i++)
    {
    	while(wg_threads[i]->busy)
    	{
    		usleep(100*1000);
    		std::cout << "Waiting!" << std::endl;
    	}
    }

    world->consistency();

    mainLoop();

	free_resources();
	return EXIT_SUCCESS;
}
