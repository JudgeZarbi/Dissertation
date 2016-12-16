/**
 * From the OpenGL Programming wikibook: http://en.wikibooks.org/wiki/OpenGL_Programming
 * This file is in the public domain.
 * Contributors: Sylvain Beucler
 */
using namespace std;
//Core libraries
#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/noise.hpp>

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
int blocktypes = 4;
GLuint program;
GLuint texarray;
GLint attribute_coord;
GLint uniform_mvp;
GLint uniform_texture;
World::World* world;

bool init_resources()
{
	//wat even is this necessary for
	GLuint VertexArrayID;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	world = new World::World();

	texarray = Texture::load_block_textures();

	program = Util::create_program("shaders/vert.glsl", "shaders/frag.glsl");

	if (!program)
	{
		return false;
	}
	
	attribute_coord = Util::get_attrib(program, "coord");
	uniform_mvp = Util::get_uniform(program, "mvp");
	uniform_texture = Util::get_uniform(program, "texarray");

	if (attribute_coord == -1 || uniform_mvp == -1 || uniform_texture == -1)
	{
		return false;		
	}	

	return true;
}

void render(SDL_Window* window)
{
	glm::mat4 view = glm::lookAt(Util::position, Util::position + Util::lookat, Util::up);
	glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 1000.0f);

	glm::mat4 mvp = projection * view;
	glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

	glClearColor(1.0, 1.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	
	glUseProgram(program);
	
	glActiveTexture(GL_TEXTURE0);
	glUniform1i(uniform_texture, /*GL_TEXTURE*/0);
	
	glEnableVertexAttribArray(attribute_coord);

	world->render(attribute_coord, uniform_mvp, mvp);	
	
	glDisableVertexAttribArray(attribute_coord);
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

	while (true) {
		SDL_Event ev;
		Util::timing();
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
					Controls::keyDown(&ev.key, &keys);					
				}
			}
			else if (ev.type == SDL_KEYUP)
			{
				Controls::keyUp(&ev.key, &keys);
			}
			else if (ev.type == SDL_MOUSEMOTION)
			{
				Controls::mouseMotion(&ev.motion, &Util::angle);
				Util::update_vectors();
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
			Util::movement(keys);			
		}
		render(window);
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
		cerr << "Error: can't create window: " << SDL_GetError() << endl;
		return EXIT_FAILURE;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	if (SDL_GL_CreateContext(window) == NULL)
	{
		cerr << "Error: SDL_GL_CreateContext: " << SDL_GetError() << endl;
		return EXIT_FAILURE;
	}

	glewExperimental = GL_TRUE;
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << endl;
		return EXIT_FAILURE;
	}
	if (!GLEW_VERSION_3_3)
	{
		cerr << "Error: your graphic card does not support OpenGL 3.3" << endl;
		return EXIT_FAILURE;
	}

	// Set relative mouse mode, this will grab the cursor.
	SDL_SetRelativeMouseMode(SDL_TRUE);

	if (!init_resources())
	{
		return EXIT_FAILURE;
	}

	Util::position = glm::vec3(0, 150, 0);
	Util::angle = glm::vec3(0, -0.5, 0);
	Util::update_vectors();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    mainLoop(window);

	free_resources();
	return EXIT_SUCCESS;
}
