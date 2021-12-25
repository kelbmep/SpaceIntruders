#define GLEW_STATIC

#include <string>
#include <iostream>
#include <GL/GLVertexBuffer.hpp>
#include <GL/GLDrawProgram.hpp>
#include <GL/GLTexture.hpp>
#include <GL/GLRender.hpp>
#include <GL/glew.h>

void check_errors(std::string file, int line)
{
	auto err(glGetError());

	while (err != GL_NO_ERROR)
	{
		std::string error;

		switch (err)
		{
		case GL_INVALID_OPERATION: error = "INVALID_OPERATION"; break;
		case GL_INVALID_ENUM: error = "INVALID_ENUM"; break;
		case GL_INVALID_VALUE: error = "INVALID_VALUE"; break;
		case GL_OUT_OF_MEMORY: error = "OUT_OF_MERORY"; break;
		case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
		default: error = "UNKNOWN"; break;
		}
		std::cerr << "GL_" << error << " : 0x" << std::hex << err << std::dec << " - " << file << ": " << line << std::endl;
		err = glGetError();
	}
}

GLRender::GLRender(const Engine& engine, SDL_Window* window)
	: _engine(engine)
	, _draw_context{ SDL_GL_CreateContext(window), SDL_GL_DeleteContext }
{
	printf("OpenGL version supported by this platform (%s): \n",
		glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glewExperimental = GL_TRUE;
	auto res = glewInit();
	if (res != GLEW_OK)
	{
		throw std::runtime_error("Glew error");
	}
	
	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void GLRender::draw()
{
	glDisable(GL_CULL_FACE);
	glDisable(GL_SCISSOR_TEST);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_SCISSOR_TEST);

	for (const auto& command : _commands)
	{
		auto gl_vertex_buffer = std::dynamic_pointer_cast<GLVertexBuffer>(command.vertex_buffer);

		if (gl_vertex_buffer)
		{
			auto gl_program = std::dynamic_pointer_cast<GLProgram>(command.program);
			if (gl_program)
			{
				gl_program->activate();
				if (command.scissor)
				{
					glScissor((GLint)command.scissor->x,
							  (GLint)(_engine.get_window_height() - command.scissor->w),
							  (GLsizei)(command.scissor->z - command.scissor->x),
							  (GLsizei)(command.scissor->w - command.scissor->y));
				}
				else
				{
					glScissor(0, 0, (GLsizei)_engine.get_window_width(), (GLsizei)_engine.get_window_height());
				}
				if (command.sub)
				{
					gl_vertex_buffer->draw(command.sub->num, command.sub->offset);
				}
				else
				{
					gl_vertex_buffer->draw();
				}
			}
		}
	}
	_commands.clear();
}

std::shared_ptr<VertexBuffer> GLRender::create_vertex_buffer(MeshData data) const
{
	return std::make_shared<GLVertexBuffer>(_engine, data);
}

std::shared_ptr<ShaderProgram> GLRender::create_program(std::string name) const
{
	if (name == "draw")
	{
		return std::make_shared<GLDrawProgram>();
	}

	return nullptr;
}

std::shared_ptr<Texture> GLRender::create_texture(Bitmap bitmap) const
{
	return std::make_shared<GLTexture>(std::move(bitmap));
}

std::shared_ptr<ParticleBuffer> GLRender::create_particle_buffer(std::vector <ParticleBuffer::ParticleData> data) const
{
	return std::make_shared<GLParticleBuffer>(std::move(data));
}
