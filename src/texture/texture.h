/**
 * Based on code at https://gitlab.com/wikibooks-opengl/modern-tutorials/tree/master/glescraft-sdl2
 * which is in the public domain.
 */
#ifndef CONTROLS_TEXTURE_H
#define CONTROLS_TEXTURE_H

#include <iostream>

#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../util/global.h"

namespace Game
{
	GLuint load_block_textures();
}

#endif