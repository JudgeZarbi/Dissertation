#ifndef _JZ_TEXTURE_H
#define _JZ_TEXTURE_H

#include <GL/glew.h>
#include "SDL.h"
#include "SDL_image.h"

#include "../util/global.h"

namespace Texture
{
	GLuint load_block_textures();
}

#endif