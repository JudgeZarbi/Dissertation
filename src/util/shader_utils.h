#ifndef _JZ_CREATE_SHADER_H
#define _JZ_CREATE_SHADER_H
#include <GL/glew.h>
#include "SDL.h"
#include <iostream>

namespace Util
{
	extern char* file_read(const char* filename);
	extern void print_log(GLuint object);
	extern GLuint create_shader(const char* filename, GLenum type);
	GLuint create_program(const char* vs, const char* fs);
	GLint get_attrib(GLuint program, const char* name);
	GLint get_uniform(GLuint program, const char* name);
}


#endif