/**
 * Based on code at https://gitlab.com/wikibooks-opengl/modern-tutorials/tree/master/glescraft-sdl2
 * which is in the public domain.
 */
#ifndef _RENDER_RENDERER_H
#define _RENDER_RENDERER_H

#include "../world/world.h"
#include "../util/shader_utils.h"
#include "../texture/texture.h"

namespace Game
{
	struct Renderer
	{
		virtual ~Renderer();

		void initialise();
		void render(World* world);
		void render(Chunk* chunk);
		void onResize(int width, int height);
		void buffer(Chunk* chunk);

		static Renderer* instance;
		GLuint program;
		GLuint texarray;
		GLint attribute_coord;
		GLint uniform_mvp;
		GLint uniform_texture;
		GLuint cursor_vbo;
		SDL_Window* window;
		int mx, my, mz;
		int face;
		bool target;
		int screen_width=800;
		int screen_height=600;
	};
}
#endif