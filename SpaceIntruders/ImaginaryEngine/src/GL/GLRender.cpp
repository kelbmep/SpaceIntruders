#include <GL/GLRender.hpp>
#include <GL/Sprite.hpp>

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
	spr.push({ 0, 0, 0.0, 1.0, 1.0, 0, 300, 1.0, 1.0, 0.0, 300, 0, 1.0, 0.0, 1.0 });
	spr.push({ 300, 0, 1.0, 0.0, 0.0, 0, 300, 0.0, 0.0, 1.0, 300, 300, 0.0, 1.0, 0.0 });
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
	CheckForErrors("GLRender.cpp", 103);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, spr.get_size() * sizeof(Engine::VertexTriangle), spr.get_data(), GL_STATIC_DRAW);
	CheckForErrors("GLRender.cpp", 108);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	CheckForErrors("GLRender.cpp", 112);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	CheckForErrors("GLRender.cpp", 116);

	glGenBuffers(1, &_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
	CheckForErrors("GLRender.cpp", 121);

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

	glUseProgram(_program);
	_uScreenSize = glGetUniformLocation(_program, "screenSize");
}

void GLRender::draw()
{
	glClearColor(0.5, 0.5, 0.5 , 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glUseProgram(_program);
	glUniform2f(_uScreenSize, static_cast<GLfloat>(_engine.get_window_width()), static_cast<GLfloat>(_engine.get_window_height()));

	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}