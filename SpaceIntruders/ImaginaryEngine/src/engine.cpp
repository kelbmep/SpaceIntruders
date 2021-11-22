#include "tiny_obj_loader.cc"
#include "engine.hpp"
#include <SDL_version.h>
#include <SDL.h>

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

void Engine::init(std::string name_window)
{
	//SDL_Surface* screen_surface = NULL;
	//SDL_Window* window = NULL;
	_pimpl->window = SDL_CreateWindow(name_window.c_str(), SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED, 1200, 800,
		SDL_WINDOW_SHOWN);
	_pimpl->renderer = SDL_CreateRenderer(_pimpl->window, -1, SDL_RENDERER_ACCELERATED);
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
}

void Engine::drawObj(const std::string inputfile, int width, int height)
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

	SDL_SetRenderDrawColor(_pimpl->renderer, 0xFF, 0x00, 0xFF, 0x00);
	// Loop over shapes
	for (size_t s = 0; s < shapes.size(); s++)
	{
		// Loop over faces(polygon)
		size_t index_offset = 0;
		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			size_t fv = size_t(shapes[s].mesh.num_face_vertices[f]);

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

				int x0 = 200 + (vx_1 + 1.) * width / 2;
				int y0 = (vy_1 + 1.) * height / 2;
				int x1 = 200 + (vx_2 + 1.) * width / 2;
				int y1 = (vy_2 + 1.) * height / 2;

				Point2D first = { x0, height - y0 };
				Point2D second = { x1, height - y1 };
				
				drawLineByPoints(&first, &second);

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
	SDL_RenderPresent(_pimpl->renderer);
}

void Engine::drawLineByPoints(int x0, int y0, int x1, int y1) 
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
			SDL_RenderDrawPoint(_pimpl->renderer, y, x);
		}
		else 
		{
			SDL_RenderDrawPoint(_pimpl->renderer, x, y);
		}
		error2 += derror2;

		if (error2 > dx) 
		{
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void Engine::drawLineByPoints(Point2D* a, Point2D* b)
{
	bool steep = false;

	if (std::abs((*a).getX() - (*b).getX()) < std::abs((*a).getY() - (*b).getY()))
	{
		int temp = (*a).getX();
		(*a).setX((*a).getY());
		(*a).setY(temp);
		temp = (*b).getX();
		(*b).setX((*b).getY());
		(*b).setY(temp);
		steep = true;
	}

	if ((*a).getX() > (*b).getX())
	{
		int temp = (*a).getX();
		(*a).setX((*b).getX());
		(*b).setX(temp);
		temp = (*a).getY();
		(*a).setY((*b).getY());
		(*b).setY(temp);
	}

	int dx = (*b).getX() - (*a).getX();
	int dy = (*b).getY() - (*a).getY();
	int derror2 = std::abs(dy) * 2;
	int error2 = 0;
	int y = (*a).getY();

	for (int x = (*a).getX(); x <= (*b).getX(); x++)
	{
		if (steep)
		{
			SDL_RenderDrawPoint(_pimpl->renderer, y, x);
		}
		else
		{
			SDL_RenderDrawPoint(_pimpl->renderer, x, y);
		}
		error2 += derror2;

		if (error2 > dx)
		{
			y += ((*b).getY() > (*a).getY() ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

Engine::~Engine() = default;

Point2D::Point2D()
{
	x = 0;
	y = 0;
}

Point2D::Point2D(int a)
{
	x = a;
	y = 0;
}

Point2D::Point2D(int a, int b)
{
	x = a;
	y = b;
}

int Point2D::getX()
{
	return this->x;
}

void Point2D::setX(int a)
{
	this->x = a;
}

int Point2D::getY()
{
	return this->y;
}

void Point2D::setY(int b)
{
	this->y = b;
}