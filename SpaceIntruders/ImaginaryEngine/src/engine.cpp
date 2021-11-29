#define GLEW_STATIC

#include "tiny_obj_loader.cc"
#include "engine.hpp"
#include <GL/glew.h>
#include <SDL_version.h>
#include <SDL.h>

class Render {
public:
	explicit Render();
	Render(SDL_Renderer*);
	void drawLineByPoints(int, int, int, int);
	void drawLineByPoints(Point2D, Point2D);
	void drawObj(std::string, int, int);
	void fillTriangle(Triangle*);
private:	
	SDL_Renderer* renderer;
};

Render::Render() 
{
	renderer = nullptr;
}

Render::Render(SDL_Renderer* r) 
{
	renderer = r;
}

void Render::drawLineByPoints(int x0, int y0, int x1, int y1)
{
	bool steep = false;

	if (std::abs(x0 - x1) < std::abs(y0 - y1))
	{
		std::swap(x0, y0);
		std::swap(x1, y1);
		steep = true;
	}

	if (x0 > x1)
	{
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = y0;

	for (int x = x0; x <= x1; x++)
	{
		if (steep)
		{
			SDL_RenderDrawPoint(renderer, y, x);
		}
		else
		{
			SDL_RenderDrawPoint(renderer, x, y);
		}
		error2 += derror2;

		if (error2 > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void Render::drawLineByPoints(Point2D a, Point2D b)
{
	bool steep = false;

	if (std::abs(a.x - b.x) < std::abs(a.y - b.y))
	{
		std::swap(a.x, a.y);
		std::swap(b.x, b.y);
		steep = true;
	}

	if (a.x > b.x)
	{
		std::swap(a.x, b.x);
		std::swap(a.y, b.y);
	}

	int dx = b.x - a.x;
	int dy = b.y - a.y;
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = a.y;

	for (int x = a.x; x <= b.x; x++)
	{
		if (steep)
		{
			SDL_RenderDrawPoint(renderer, y, x);
		}
		else
		{
			SDL_RenderDrawPoint(renderer, x, y);
		}
		error2 += derror2;

		if (error2 > dx)
		{
			y += (b.y > a.y ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void Render::fillTriangle(Triangle* abc)
{
	if (abc->getFirst().y > abc->getSecond().y)
	{
		Point2D temp = abc->getFirst();
		abc->setFirst(abc->getSecond());
		abc->setSecond(temp);
	}
	if (abc->getFirst().y > abc->getThird().y)
	{
		Point2D temp = abc->getFirst();
		abc->setFirst(abc->getThird());
		abc->setThird(temp);
	}
	if (abc->getSecond().y > abc->getThird().y)
	{
		Point2D temp = abc->getSecond();
		abc->setSecond(abc->getThird());
		abc->setThird(temp);
	}

	int total_height = abc->getThird().y - abc->getFirst().y;

	for (int i = 0; i < total_height; i++)
	{
		int diff = abc->getSecond().y - abc->getFirst().y;
		bool second_half = i > diff || abc->getFirst().y == abc->getSecond().y;
		int segment_height = second_half ? abc->getThird().y - abc->getSecond().y : diff;
		double alpha = i * 1. / total_height;
		double beta = (i - (second_half ? diff : 0)) * 1. / segment_height;
		
		Point2D A =  Point2D{ abc->getFirst().x + (int)(alpha * (abc->getThird().x - abc->getFirst().x)), 
						abc->getFirst().y + (int)(alpha * (abc->getThird().y - abc->getFirst().y)) };

		Point2D B = second_half ? Point2D{ abc->getSecond().x + (int)(beta * (abc->getThird().x - abc->getSecond().x)), 
									abc->getSecond().y + (int)(beta * (abc->getThird().y - abc->getSecond().y)) } :
								Point2D{ abc->getFirst().x + (int)(beta * (abc->getSecond().x - abc->getFirst().x)), 
									abc->getFirst().y + (int)(beta * (abc->getSecond().y - abc->getFirst().y)) };
		if (A.x > B.x)
		{
			Point2D temp = A;
			A = B;
			B = temp;
		}
		for (int j = A.x; j <= B.x; j++)
		{
			SDL_RenderDrawPoint(renderer, j, abc->getFirst().y + i);
		}
	}
}

void Render::drawObj(std::string inputfile, int height, int width) 
{
	tinyobj::ObjReaderConfig reader_config;
	reader_config.mtl_search_path = "./"; // Path to material files

	tinyobj::ObjReader reader;

	if (!reader.ParseFromFile(inputfile, reader_config))
	{
		if (!reader.Error().empty())
		{
			std::cerr << "TinyObjReader: " << reader.Error();
		}
		exit(1);
	}

	if (!reader.Warning().empty())
	{
		std::cout << "TinyObjReader: " << reader.Warning();
	}

	auto& attrib = reader.GetAttrib();
	auto& shapes = reader.GetShapes();
	//auto& materials = reader.GetMaterials();

	SDL_SetRenderDrawColor(renderer, 0xFF, 0x00, 0xFF, 0x00);
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);
			Triangle abc;

			// Loop over vertices in the face.
			for (size_t v = 0; v < fv; v++)
			{
				// access to vertex
				tinyobj::index_t idx_1 = shapes[s].mesh.indices[index_offset + v];
				tinyobj::real_t vx_1 = attrib.vertices[3 * size_t(idx_1.vertex_index) + 0];
				tinyobj::real_t vy_1 = attrib.vertices[3 * size_t(idx_1.vertex_index) + 1];

				tinyobj::index_t idx_2 = shapes[s].mesh.indices[index_offset + (v + 1) % 3];
				tinyobj::real_t vx_2 = attrib.vertices[3 * size_t(idx_2.vertex_index) + 0];
				tinyobj::real_t vy_2 = attrib.vertices[3 * size_t(idx_2.vertex_index) + 1];

				int x0 = static_cast <int>(200 + (vx_1 + 1.) * width / 2);
				int y0 = static_cast <int>((vy_1 + 1.) * height / 2);
				int x1 = static_cast <int>(200 + (vx_2 + 1.) * width / 2);
				int y1 = static_cast <int>((vy_2 + 1.) * height / 2);

				Point2D first = { x0, height - y0 };
				Point2D second = { x1, height - y1 };

				if (v == 1)
				{
					abc.setFirst(first);
				}
				if (v == 2)
				{
					abc.setSecond(first);
					abc.setThird(second);
					SDL_SetRenderDrawColor(renderer, rand() % 255, rand() % 255, rand() % 255, 0x00);
					fillTriangle(&abc);
				}
				//drawLineByPoint2Ds(&first, &second);

				//tinyobj::real_t vz = attrib.vertices[3 * size_t(idx.vertex_index) + 2];

				// Check if `normal_index` is zero or positive. negative = no normal data
				/*if (idx.normal_index >= 0) {
					tinyobj::real_t nx = attrib.normals[3 * size_t(idx.normal_index) + 0];
					tinyobj::real_t ny = attrib.normals[3 * size_t(idx.normal_index) + 1];
					tinyobj::real_t nz = attrib.normals[3 * size_t(idx.normal_index) + 2];
				}

				// Check if `texcoord_index` is zero or positive. negative = no texcoord data
				if (idx.texcoord_index >= 0) {
					tinyobj::real_t tx = attrib.texcoords[2 * size_t(idx.texcoord_index) + 0];
					tinyobj::real_t ty = attrib.texcoords[2 * size_t(idx.texcoord_index) + 1];
				}
				*/
				// Optional: vertex colors
				// tinyobj::real_t red   = attrib.colors[3*size_t(idx.vertex_index)+0];
				// tinyobj::real_t green = attrib.colors[3*size_t(idx.vertex_index)+1];
				// tinyobj::real_t blue  = attrib.colors[3*size_t(idx.vertex_index)+2];
			}
			index_offset += fv;

			// per-face material
			//shapes[s].mesh.material_ids[f];
		}
	}
	SDL_RenderPresent(renderer);
}

struct Engine::Pimpl
{
	SDL_Surface* screen_surface;
	SDL_Window* window;
	bool isActive;
	SDL_Renderer* renderer;
};

Engine::Engine()
{
	_pimpl = std::make_unique<Engine::Pimpl>();
	_pimpl->screen_surface = NULL;
	_pimpl->window = NULL;
	_pimpl->isActive = true;
	_pimpl->renderer = NULL;
}

void Engine::init()
{
	SDL_version version = { 0, 0, 0 };
	SDL_GetVersion(&version);
	//_pimpl->renderer = SDL_CreateRenderer(_pimpl->window, -1, SDL_RENDERER_ACCELERATED);
	printf("SDL version : %d.%d.%d", version.major, version.minor, version.patch);
}

GLuint _VAO;
GLuint _uScreenSize;
GLuint _program;

void LoadShaderFromFile(std::string file_path, GLuint& _Shader)
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

void Engine::init(std::string name_window)
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	
	//Uint32 windowsFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	
	_pimpl->window = SDL_CreateWindow(name_window.c_str(), SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 1200, 800,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	//glewExperimental = GL_TRUE;
	auto context = SDL_GL_CreateContext(_pimpl->window);

	GLenum res = glewInit();
	if (res != GLEW_OK)
	{
		throw std::runtime_error("Glew error");
	}

	_pimpl->renderer = SDL_CreateRenderer(_pimpl->window, -1, SDL_RENDERER_ACCELERATED);

	struct Vertex 
	{
		float x;
		float y;
		float r;
		float g;
		float b;
	};

	struct VertexTriangle
	{
		Vertex arr[3];
	};

	VertexTriangle t{ 200, 200, 1.0, 1.0, 0.0, 400, 500, 1.0, 0.0, 1.0, 600, 300, 0.0, 1.0, 1.0 };
	//glVertexAttribPointer(номер поля, количество компонент, тип, 
	//нормализация(обрезка значения от -1 до 1), страйд(расстояние между двумя элементами массива), смещение от начала структуры);
	
	GLuint _VBO, _IBO;
	uint32_t indexes[3] = { 0, 1, 2 };
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);
	CheckForErrors("engine.cpp", 382);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex), &t, GL_STATIC_DRAW);
	CheckForErrors("engine.cpp", 387);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>(offsetof(Vertex, x)));
	CheckForErrors("engine.cpp", 391);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2 * sizeof(float)));
	CheckForErrors("engine.cpp", 395);

	glGenBuffers(1, &_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
	CheckForErrors("engine.cpp", 400);

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
	
	//SDL_Surface* screen_surface = NULL;
	//SDL_Window* window = NULL;	
}

bool Engine::isActive()
{
	return _pimpl->isActive;
}

void Engine::update()
{
	SDL_Event e;
	SDL_PollEvent(&e);
	if (e.type == SDL_QUIT)
	{
		_pimpl->isActive = false;
	}

	glClearColor(1, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(_program);
	glUniform2f(_uScreenSize, 800, 800);
	
	glBindVertexArray(_VAO);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	
	SDL_GL_SwapWindow(_pimpl->window);
}

void Engine::drawObj(std::string inputfile, int h, int w)
{
	Render r = Render(_pimpl->renderer);
	r.drawObj(inputfile, h, w);
}

void Engine::CheckForErrors(std::string file, int line)
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

Engine::~Engine() = default;

Triangle::Triangle()
{
	first = { 0, 0 };
	second = { 0, 0 };
	third = { 0, 0 };
}

Triangle::Triangle(Point2D a, Point2D b, Point2D c)
{
	first = a;
	second = b;
	third = c;
}

Point2D Triangle::getFirst()
{
	return this->first;
}

Point2D Triangle::getSecond()
{
	return this->second;
}

Point2D Triangle::getThird()
{
	return this->third;
}

void Triangle::setFirst(Point2D a)
{
	this->first = a;
}

void Triangle::setSecond(Point2D a)
{
	this->second = a;
}

void Triangle::setThird(Point2D a)
{
	this->third = a;
}
