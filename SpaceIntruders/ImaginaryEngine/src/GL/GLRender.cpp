#define GLEW_STATIC

/*#include <iostream>
#include <bitmap.hpp>
#include <memory>
//#include <GL/GLRender.hpp>
#include "GLRender.hpp"
#include <GL/GLVertexBuffer.hpp>
#include <GL/GLDrawProgram.hpp>
#include <SDL.h>
#include <GL/glew.h>*/
#include <fstream>
#include <string>
#include <iostream>
#include <bitmap.hpp>
#include <GL/GLVertexBuffer.hpp>
#include <GL/GLDrawProgram.hpp>
#include "sprite.hpp"
#include "GLRender.hpp"
#include <GL/glew.h>

void check_errors(std::string file, int line)
{
	GLenum err(glGetError());

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
	, _drawContext{ SDL_GL_CreateContext(window), SDL_GL_DeleteContext }
{
	printf("OpenGL version supported by this platform (%s): \n",
		glGetString(GL_VERSION));

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glewExperimental = GL_TRUE;
	GLenum res = glewInit();
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
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (const auto& command : _commands)
	{
		auto glVertexBuffer = std::dynamic_pointer_cast<GLVertexBuffer>(command.vertexBuffer);

		if (glVertexBuffer)
		{
			auto glProgram = std::dynamic_pointer_cast<GLProgram>(command.program);
			if (glProgram)
			{
				glProgram->activate();
				glVertexBuffer->draw();
			}
		}
	}
	_commands.clear();
}

void GLRender::draw(size_t count, size_t pos)
{
	glDisable(GL_CULL_FACE);
	glClearColor(1.0f, 0.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	for (const auto& command : _commands)
	{
		auto glVertexBuffer = std::dynamic_pointer_cast<GLVertexBuffer>(command.vertexBuffer);

		if (glVertexBuffer)
		{
			auto glProgram = std::dynamic_pointer_cast<GLProgram>(command.program);
			if (glProgram)
			{
				glProgram->activate();
				glVertexBuffer->draw(count, pos);
			}
		}
	}
	_commands.clear();
}

/*std::shared_ptr<VertexBuffer> GLRender::create_vertex_buffer(MeshData data) const
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
}*/

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
/*
#include <GL/GLRender.hpp>
#include <Sprite.hpp>
#include <stbi/stb_image.h>

void CheckForErrors(std::string file, int line)
{
	GLenum err(glGetError());

	while (err != GL_NO_ERROR)
	{
		std::string error;

		switch (err)
		{
		case GL_INVALID_OPERATION:error = "INVALID_OPERATION";
			break;
		case GL_INVALID_ENUM:error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:error = "INVALID_VALUE";
			break;
		case GL_OUT_OF_MEMORY:error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		default:error = "UNKNOWN";
			break;
		}
		std::cerr << "GL_" << error << " : 0x" << std::hex << err << std::dec << " - " << file << ":" << line << std::endl;

		err = glGetError();
	}
}

void GLRender::LoadShaderFromFile(std::string file_path, GLuint& _Shader)
{
	std::ifstream shader(file_path);
	std::string a = file_path.substr(file_path.length() - 5, file_path.length());
	if (!strcmp(a.c_str(), ".vert") || !strcmp(a.c_str(), ".frag"))
	{
		std::string str_shader, line;
		while (getline(shader, line))
		{
			str_shader += line + "\n";
		}
		shader.close();

		if (!strcmp(a.c_str(), ".vert"))
		{
			_Shader = glCreateShader(GL_VERTEX_SHADER);
		}
		else if (!strcmp(a.c_str(), ".frag"))
		{
			_Shader = glCreateShader(GL_FRAGMENT_SHADER);
		}
		const char* shader_c_str = str_shader.c_str();
		glShaderSource(_Shader, 1, &shader_c_str, nullptr);
		glCompileShader(_Shader);

		GLint success;
		GLchar infoLog[512];
		glGetShaderiv(_Shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			glGetShaderInfoLog(_Shader, 512, nullptr, infoLog);
			std::cerr << "ERROR::SHADER::COMPILATION_FAILED " << str_shader << "\n" << infoLog << std::endl;
		}
	}
	else
	{
		std::cerr << "Filename extension must be .vert or .frag" << std::endl;
	}
}

GLRender::GLRender(const Engine& engine, SDL_Window* window)
    : _engine(engine)
    , _drawContext{ SDL_GL_CreateContext(window), SDL_GL_DeleteContext }
{
    printf("OpenGL version supported by this platform (%s): \n",
        glGetString(GL_VERSION));
	
    glewExperimental = GL_TRUE;
    GLenum res = glewInit();
    if (res != GLEW_OK)
    {
        throw std::runtime_error("Glew error");
    }

	Sprite spr;
	spr.push({ 0, 0, 0.0, 1.0, 1.0, 0, 0, 
			   0, 300, 1.0, 1.0, 0.0, 0, 300, 
			   300, 0, 1.0, 0.0, 1.0, 300, 0 });

	spr.push({ 300, 0, 1.0, 0.0, 0.0, 300, 0, 
			   0, 300, 0.0, 0.0, 1.0, 0, 300, 
			   300, 300, 0.0, 1.0, 0.0, 300, 300 });

	spr.Transform(1.0, 12, { 100, 100 });
	spr.Transform(2.0, 5, { -200, -200 });

	std::cout << spr.get_rotation();
	//Engine::VertexTriangle t{ 200, 200, 1.0, 1.0, 0.0, 400, 500, 1.0, 0.0, 1.0, 600, 300, 0.0, 1.0, 1.0 };
	//glVertexAttribPointer(номер поля, количество компонент, тип, 
	//нормализация(обрезка значения от -1 до 1), страйд(расстояние между двумя элементами массива), смещение от начала структуры);

	GLuint _VBO, _IBO;
	uint32_t indexes[6] = { 0, 1, 2, 3, 4, 5 };
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	CheckForErrors("GLRender.cpp", 109);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, spr.get_size() * sizeof(Engine::VertexTriangle), spr.get_data(), GL_STATIC_DRAW);
	CheckForErrors("GLRender.cpp", 114);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
	CheckForErrors("GLRender.cpp", 118);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	CheckForErrors("GLRender.cpp", 121);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
	CheckForErrors("GLRender.cpp", 126);

	glGenBuffers(1, &_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUvertFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
	CheckForErrors("GLRender.cpp", 131);

	GLuint _vertexShader, _fragmentShader;
	LoadShaderFromFile("../../../../SpaceIntruders/ImaginaryEngine/src/shader_vert.vert", _vertexShader);
	LoadShaderFromFile("../../../../SpaceIntruders/ImaginaryEngine/src/shader_frag.frag", _fragmentShader);

	_program = glCreateProgram();

	glAttachShader(_program, _vertexShader);
	glAttachShader(_program, _fragmentShader);
	glLinkProgram(_program);

	GLint success;
	GLchar infoLog[512];

	glGetProgramiv(_program, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(_program, 512, nullptr, infoLog);
		std::cerr << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
	}

	std::string filepath = "C://Users/user/Desktop/lab1.jpg";

	SDL_RWops* file = SDL_RWFromFile(filepath.data(), "rb");
	if (file == nullptr)
	{
		throw std::runtime_error("Failed to open file: " + std::string(filepath));
	}
	Sint64 length = SDL_RWsize(file);
	std::vector<unsigned char> data(length);

	SDL_RWread(file, data.data(), length, 1);
	SDL_RWclose(file);

	int width, height, ch;
	stbi_set_flip_vertically_on_load(false);
	auto img = stbi_load_from_memory(data.data(), data.size(), &width, &height, &ch, 0);

	std::uint32_t id;
	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);

	auto internal_format = [&]()
	{
		switch (ch)
		{
		case 1: return GL_R8;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		}
	}();

	auto format = [&]()
	{
		switch (ch)
		{
		case 1: return GL_RED;
		case 2: return GL_RG;
		case 3: return GL_RGB;
		case 4: return GL_RGBA;
		}
	}();

	glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, GL_UNSIGNED_BYTE, img);

	if (img == nullptr)
	{
		throw std::runtime_error("Unsupported file format: " + std::string(filepath));
	}
	stbi_image_free(img);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);
	
	glUseProgram(_program);
	_uScreenSize = glGetUniformLocation(_program, "screenSize");
	_location = glGetUniformLocation(_program, "uTexture");
}

void GLRender::draw()
{
	glClearColor(0.5, 0.5, 0.5 , 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(_program);
	glUniform2f(_uScreenSize, static_cast<GLfloat>(_engine.get_window_width()), static_cast<GLfloat>(_engine.get_window_height()));
	glUniform1i(_location, 0);
	
	//glUniform2f()
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}*/