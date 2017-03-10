#include "renderer.h"

namespace Game
{
	void Renderer::initialise()
	{
		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow("Many chunks and controls!",
			SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
			screen_width, screen_height,
			SDL_WINDOW_RESIZABLE | SDL_WINDOW_OPENGL);
		if (window == NULL)
		{
			std::cerr << "Error: can't create window: " << SDL_GetError() << std::endl;
			return;
		}

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		if (SDL_GL_CreateContext(window) == NULL)
		{
			std::cerr << "Error: SDL_GL_CreateContext: " << SDL_GetError() << std::endl;
			return;
		}

		glewExperimental = GL_TRUE;
		GLenum glew_status = glewInit();
		if (glew_status != GLEW_OK)
		{
			std::cerr << "Error: glewInit: " << glewGetErrorString(glew_status) << std::endl;
			return;
		}
		if (!GLEW_VERSION_3_3)
		{
			std::cerr << "Error: your graphic card does not support OpenGL 3.3" << std::endl;
			return;
		}

		// Set relative mouse mode, this will grab the cursor.
		SDL_SetRelativeMouseMode(SDL_TRUE);

	    glEnable(GL_DEPTH_TEST);
	    glEnable(GL_CULL_FACE);

		//wat even is this necessary for
		GLuint VertexArrayID;
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		texarray = load_block_textures();

		program = create_program("shaders/vert.glsl", "shaders/frag.glsl");

		if (!program)
		{
			return;
		}
		
		attribute_coord = get_attrib(program, "coord");
		uniform_mvp = get_uniform(program, "mvp");
		uniform_texture = get_uniform(program, "texarray");

		if (attribute_coord == -1 || uniform_mvp == -1 || uniform_texture == -1)
		{
			return;		
		}	

		glPolygonOffset(1, 1);
		glEnableVertexAttribArray(attribute_coord);
		glClearColor(1.0, 1.0, 1.0, 1.0);
		glUseProgram(program);
		glGenBuffers(1, &cursor_vbo);
		
		glActiveTexture(GL_TEXTURE0);
		glUniform1i(uniform_texture, /*GL_TEXTURE*/0);
	}

	void Renderer::render(World* world)
	{
		glm::mat4 view = glm::lookAt(position, position + lookat, up);
		glm::mat4 projection = glm::perspective(45.0f, 1.0f*screen_width/screen_height, 0.1f, 1000.0f);

		glm::mat4 pv = projection * view;
		glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(pv));

		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_POLYGON_OFFSET_FILL);

		int rendered = 0;
		int cur_x = floor(position.x);
		int cur_z = floor(position.z);
		int cur_x_chunk = (((((cur_x >= 0 ? cur_x : cur_x - 15) / X) + CHUNKS_RANGE) % CHUNKS_X) + CHUNKS_X) % CHUNKS_X;
		int cur_z_chunk = (((((cur_z >= 0 ? cur_z : cur_z - 15) / Z) + CHUNKS_RANGE) % CHUNKS_Z) + CHUNKS_Z) % CHUNKS_Z;

		for(int x = ((cur_x_chunk - FRAME_RANGE) >= 0 ? cur_x_chunk - FRAME_RANGE : (cur_x_chunk - FRAME_RANGE + CHUNKS_X)); x != ((cur_x_chunk + FRAME_RANGE + 1) % CHUNKS_X); x = ((x + 1) % CHUNKS_X))
		{
			for(int y = 0; y < CHUNKS_Y; y++)
			{
				for(int z = ((cur_z_chunk - FRAME_RANGE) >= 0 ? cur_z_chunk - FRAME_RANGE : (cur_z_chunk - FRAME_RANGE + CHUNKS_Z)); z != ((cur_z_chunk + FRAME_RANGE + 1) % CHUNKS_Z); z = ((z + 1) % CHUNKS_Z))
				{
					std::cout << "Render loop!" << std::endl;
					Chunk* chunk = world->chunks[x][y][z];
					if (!chunk->vbo)
					{
						glGenBuffers(1, &(chunk->vbo));
					}
					glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(world->chunks[x][y][z]->cx * X, world->chunks[x][y][z]->cy * Y, world->chunks[x][y][z]->cz * Z));
					glm::mat4 mvp = pv * model;

					// Is this chunk on the screen?
					glm::vec4 center = mvp * glm::vec4(world->chunks[x][y][z]->cx / 2, world->chunks[x][y][z]->cx / 2, world->chunks[x][y][z]->cx / 2, 1);

					center.x /= center.w;
					center.y /= center.w;

					// If it is behind the camera, don't bother drawing it
					if(center.z < -Y / 2)
					{
						std::cout << "lolno" << std::endl;
						continue;
					}

					// If it is outside the screen, don't bother drawing it
					if(fabsf(center.x) > 1 + fabsf(Y * 2 / center.w) || fabsf(center.y) > 1 + fabsf(Y * 2 / center.w))
					{
						continue;
					}

					glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(mvp));

					render(world->chunks[x][y][z]);
					rendered += 1;
				}
			}
		}
		
		/* At which voxel are we looking? */
		/* Very naive ray casting algorithm to find out which block we are looking at */

		glm::vec3 testpos = position;
		glm::vec3 prevpos = position;

		for(int i = 0; i < 100; i++) {
			/* Advance from our currect position to the direction we are looking at, in small steps */

			prevpos = testpos;
			testpos += lookat * 0.1f;

			mx = floorf(testpos.x);
			my = floorf(testpos.y);
			mz = floorf(testpos.z);	

			/* If we find a block that is not air, we are done */

			if(world->get(mx, my, mz))
			{
				target = true;	
				break;		
			}
		}

		if(!world->get(mx, my, mz))
		{
			target = false;
		}

		glDisable(GL_POLYGON_OFFSET_FILL);
		glDisable(GL_CULL_FACE);
		glBindBuffer(GL_ARRAY_BUFFER, cursor_vbo);
		glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(pv));

		if(target)
		{
			/* Find out which face of the block we are looking at */
			int px = floorf(prevpos.x);
			int py = floorf(prevpos.y);
			int pz = floorf(prevpos.z);

			if(px > mx)
				face = 0;
			else if(px < mx)
				face = 3;
			else if(py > my)
				face = 1;
			else if(py < my)
				face = 4;
			else if(pz > mz)
				face = 2;
			else if(pz < mz)
				face = 5;

			float bx = mx;
			float by = my;
			float bz = mz;

			/* Render a box around the block we are pointing at */

			float box[24][4] = {
				{bx + 0, by + 0, bz + 0, 8},
				{bx + 1, by + 0, bz + 0, 8},
				{bx + 0, by + 1, bz + 0, 8},
				{bx + 1, by + 1, bz + 0, 8},
				{bx + 0, by + 0, bz + 1, 8},
				{bx + 1, by + 0, bz + 1, 8},
				{bx + 0, by + 1, bz + 1, 8},
				{bx + 1, by + 1, bz + 1, 8},

				{bx + 0, by + 0, bz + 0, 8},
				{bx + 0, by + 1, bz + 0, 8},
				{bx + 1, by + 0, bz + 0, 8},
				{bx + 1, by + 1, bz + 0, 8},
				{bx + 0, by + 0, bz + 1, 8},
				{bx + 0, by + 1, bz + 1, 8},
				{bx + 1, by + 0, bz + 1, 8},
				{bx + 1, by + 1, bz + 1, 8},

				{bx + 0, by + 0, bz + 0, 8},
				{bx + 0, by + 0, bz + 1, 8},
				{bx + 1, by + 0, bz + 0, 8},
				{bx + 1, by + 0, bz + 1, 8},
				{bx + 0, by + 1, bz + 0, 8},
				{bx + 0, by + 1, bz + 1, 8},
				{bx + 1, by + 1, bz + 0, 8},
				{bx + 1, by + 1, bz + 1, 8},
			};

			glBufferData(GL_ARRAY_BUFFER, sizeof(box), box, GL_DYNAMIC_DRAW);
			glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
			glDrawArrays(GL_LINES, 0, 24);		
		}

		/* Draw a cross in the center of the screen */

		float cross[4][4] = {
			{-0.05, 0, 0, 9},
			{+0.05, 0, 0, 9},
			{0, -0.05, 0, 9},
			{0, +0.05, 0, 9},
		};

		glDisable(GL_DEPTH_TEST);
		glm::mat4 one(1);
		glUniformMatrix4fv(uniform_mvp, 1, GL_FALSE, glm::value_ptr(one));
		glBufferData(GL_ARRAY_BUFFER, sizeof(cross), cross, GL_DYNAMIC_DRAW);
		glVertexAttribPointer(attribute_coord, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glDrawArrays(GL_LINES, 0, 4);

		/* And we are done */

		SDL_GL_SwapWindow(window);

	}

	void Renderer::render(Chunk* chunk)
	{
		if(chunk->changed)
		{
			chunk->build_vertices();
			glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
			glBufferData(GL_ARRAY_BUFFER, chunk->elements * sizeof(vertex), chunk->vertices, GL_STATIC_DRAW);
			delete[] chunk->vertices;
		}
//		if(!(chunk->elements))
//		{
//			std::cout << "lolwat" << std::endl;
//			return;
//		}
		glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
		glVertexAttribPointer(attribute_coord, 4, GL_UNSIGNED_BYTE, GL_FALSE, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, chunk->elements);
	}

	void Renderer::onResize(int width, int height)
	{
//		screen_width = width;
//		screen_height = height;
		glViewport(0, 0, screen_width, screen_height);
	}

}