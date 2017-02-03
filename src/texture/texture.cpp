#include "texture.h"

namespace Texture 
{
	GLuint load_block_textures()
	{
		GLuint tex_id;
		glGenTextures(1, &tex_id);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D_ARRAY, tex_id);
		// Texture parameters, should be consistent, may need to investigate others, eg mipmap
	  	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	 	glTexParameteri(GL_TEXTURE_2D_ARRAY,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

		glTexImage3D( GL_TEXTURE_2D_ARRAY, 	//type
	        0, 								//??? probably mipmap level
	        GL_RGBA, 						//image type
	        256, 							//height - should be parameterised
	        256,							//width - should be parameterised
	        blocktypes + 1, 				//layers, +1 because we consider "air" a block type and include an empty texture for it at 0.
	        0, 								//should be 0
	        GL_RGBA, 						//image type
	        GL_UNSIGNED_BYTE, 				//data type
	        NULL); 							//image data, null because subimages

		for(int i = 0; i <= blocktypes; i++)
		{
			SDL_Surface* tex = IMG_Load(("textures/tex_" + std::to_string(i) + ".png").data());
			if (tex == NULL) {
				continue;
			}

		    glTexSubImage3D( 
		    	GL_TEXTURE_2D_ARRAY,//image type
			    0,                 	//mipmap number - 0 because we're autogenerating them.
			    0,				   	//xoffset - should always be 0
			    0,					//yoffset - should always be 0
			    i,                 	//zoffset - the "layer" of the current texture
			    256,				//width - should be parameterised
			    256,				//height - should be parameterised
			    1,                 	//depth - should always be 1, we're only loading in a single image at once
			    GL_RGBA,           	//image format
			    GL_UNSIGNED_BYTE,  	//data format
			    tex->pixels);       //data pointer

			SDL_FreeSurface(tex);
		}
		return tex_id;
	}
}

