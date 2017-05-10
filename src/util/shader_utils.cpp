/**
 * Code from https://gitlab.com/wikibooks-opengl/modern-tutorials/tree/master/glescraft-sdl2
 * which is in the public domain.
 */
/* Using SDL2 for the base window and OpenGL context init */
#include "shader_utils.h"

namespace Game
{
	/**
	 * Store all the file's contents in memory, useful to pass shaders
	 * source code to OpenGL.  Using SDL_RWops for Android asset support.
	 */
	char* file_read(const char* filename) {
		SDL_RWops *rw = SDL_RWFromFile(filename, "rb");
		if (rw == NULL) return NULL;
		
		Sint64 res_size = SDL_RWsize(rw);
		char* res = (char*)malloc(res_size + 1);

		Sint64 nb_read_total = 0, nb_read = 1;
		char* buf = res;
		while (nb_read_total < res_size && nb_read != 0) {
			nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
			nb_read_total += nb_read;
			buf += nb_read;
		}
		SDL_RWclose(rw);
		if (nb_read_total != res_size) {
			free(res);
			return NULL;
		}
		
		res[nb_read_total] = '\0';
		return res;
	}

	char* data_read(const char* filename) {
		SDL_RWops *rw = SDL_RWFromFile(filename, "r");
		if (rw == NULL) return NULL;
		
		Sint64 res_size = SDL_RWsize(rw);
		char* res = (char*)malloc(res_size + 1);

		Sint64 nb_read_total = 0, nb_read = 1;
		char* buf = res;
		while (nb_read_total < res_size && nb_read != 0) {
			nb_read = SDL_RWread(rw, buf, 1, (res_size - nb_read_total));
			nb_read_total += nb_read;
			buf += nb_read;
		}
		SDL_RWclose(rw);
		if (nb_read_total != res_size) {
			free(res);
			return NULL;
		}
		
		res[nb_read_total] = '\0';
		return res;
	}


	/**
	 * Display compilation errors from the OpenGL shader compiler
	 */
	void print_log(GLuint object) {
		GLint log_length = 0;
		if (glIsShader(object)) {
			glGetShaderiv(object, GL_INFO_LOG_LENGTH, &log_length);
		} else if (glIsProgram(object)) {
			glGetProgramiv(object, GL_INFO_LOG_LENGTH, &log_length);
		} else {
			std::cerr << "printlog: Not a shader or a program" << std::endl;
			return;
		}

		char* log = (char*)malloc(log_length);
		
		if (glIsShader(object))
			glGetShaderInfoLog(object, log_length, NULL, log);
		else if (glIsProgram(object))
			glGetProgramInfoLog(object, log_length, NULL, log);
		
		std::cerr << log << std::endl;
		free(log);
	}

	/**
	 * Compile the shader from file 'filename', with error handling
	 */
	GLuint create_shader(const char* filename, GLenum type) {
		const GLchar* source = file_read(filename);
		if (source == NULL) {
			std::cerr << "Error opening " << filename << ": " << SDL_GetError() << std::endl;
			return 0;
		}
		GLuint res = glCreateShader(type);
		const GLchar* sources[] = {source};
		glShaderSource(res, 1, sources, NULL);
		free((void*)source);
		
		glCompileShader(res);
		GLint compile_ok = GL_FALSE;
		glGetShaderiv(res, GL_COMPILE_STATUS, &compile_ok);
		if (compile_ok == GL_FALSE) {
			std::cerr << filename << ":";
			print_log(res);
			glDeleteShader(res);
			return 0;
		}
		
		return res;
	}

	GLuint create_program(const char* vs, const char* fs)
	{
		GLint link_ok = GL_FALSE;
		
		GLuint program = glCreateProgram();
		GLuint shader;

		if (vs)
		{
			shader = create_shader(vs, GL_VERTEX_SHADER);
			if (!shader)
			{
				return 0;
			}
			glAttachShader(program, shader);
		}
		if (fs)
		{
			shader = create_shader(fs, GL_FRAGMENT_SHADER);
			if (!shader)
			{
				return 0;
			}
			glAttachShader(program, shader);
		}
		
		glLinkProgram(program);
		glGetProgramiv(program, GL_LINK_STATUS, &link_ok);
		if (!link_ok) {
			std::cerr << "glLinkProgram:";
			print_log(program);
			return 0;
		}
		return program;
	}

	GLint get_attrib(GLuint program, const char* name)
	{
		GLint attrib = glGetAttribLocation(program, name);
		if (attrib == -1) {
			std::cerr << "Could not bind attribute " << name << std::endl;
		}
		return attrib;
	}

	GLint get_uniform(GLuint program, const char* name)
	{
		GLint uniform = glGetUniformLocation(program, name);
		if (uniform == -1) {
			std::cerr << "Could not bind uniform " << name << std::endl;
		}
		return uniform;
	}
}

