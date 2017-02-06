#ifndef CONTROLS_TEXTURE_H
#define CONTROLS_TEXTURE_H

#include <iostream>

#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../util/global.h"

namespace Texture
{
	GLuint load_block_textures();
}

#endif