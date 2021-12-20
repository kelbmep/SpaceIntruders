#include <SDL/SDLRender.hpp>

SDLRender::SDLRender(const Engine& engine, std::shared_ptr<SDL_Window> sdl_window)
	: _engine{ engine }
	, _sdl_window(std::move(sdl_window))
	, _sdl_render{ SDL_CreateRenderer(_sdl_window.get(), -1, 0), SDL_DestroyRenderer }
{
}

void SDLRender::draw()
{
	auto screen_width = static_cast<int>(_engine.get_window_width());
	auto screen_height = static_cast<int>(_engine.get_window_height());
	auto difference = 0;
	if (screen_width > screen_height)
	{
		difference = screen_width - screen_height;
	}
	auto obj_width = screen_width - difference;
	auto obj_height = screen_height;

	SDL_RenderPresent(_sdl_render.get());
}

std::shared_ptr<ShaderProgram> SDLRender::create_program(std::string name) const
{
	return nullptr;
}

std::shared_ptr<VertexBuffer> SDLRender::create_vertex_buffer(MeshData data) const
{
	return nullptr;
}

std::shared_ptr<Texture> SDLRender::create_texture(Bitmap bitmap) const
{
	return nullptr;
}

void SDLRender::draw_line_by_points(int x0, int y0, int x1, int y1)
{
	auto steep = false;

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

	auto dx = x1 - x0;
	auto dy = y1 - y0;
	auto derror2 = std::abs(dy) * 2;
	auto error2 = 0;
	auto y = y0;

	for (auto x = x0; x <= x1; x++)
	{
		if (steep)
		{
			SDL_RenderDrawPoint(_sdl_render.get(), y, x);
		}
		else
		{
			SDL_RenderDrawPoint(_sdl_render.get(), x, y);
		}
		error2 += derror2;

		if (error2 > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void SDLRender::draw_line_by_points(Point2D a, Point2D b)
{
	auto steep = false;

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

	auto dx = b.x - a.x;
	auto dy = b.y - a.y;
	auto derror2 = std::abs(dy) * 2;
	auto error2 = 0;
	auto y = a.y;

	for (int x = a.x; x <= b.x; x++)
	{
		if (steep)
		{
			SDL_RenderDrawPoint(_sdl_render.get(), y, x);
		}
		else
		{
			SDL_RenderDrawPoint(_sdl_render.get(), x, y);
		}
		error2 += derror2;

		if (error2 > dx)
		{
			y += (b.y > a.y ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void SDLRender::fill_triangle(Triangle* abc)
{
	SDL_SetRenderDrawColor(_sdl_render.get(), rand() % 256, rand() % 256, rand() % 256, rand() % 256);
	if (abc->get_first().y > abc->get_second().y)
	{
		Point2D temp = abc->get_first();
		abc->set_first(abc->get_second());
		abc->set_second(temp);
	}
	if (abc->get_first().y > abc->get_third().y)
	{
		Point2D temp = abc->get_first();
		abc->set_first(abc->get_third());
		abc->set_third(temp);
	}
	if (abc->get_second().y > abc->get_third().y)
	{
		Point2D temp = abc->get_second();
		abc->set_second(abc->get_third());
		abc->set_third(temp);
	}

	auto total_height = abc->get_third().y - abc->get_first().y;

	for (auto i = 0; i < total_height; i++)
	{
		auto diff = abc->get_second().y - abc->get_first().y;
		auto second_half = i > diff || abc->get_first().y == abc->get_second().y;
		auto segment_height = second_half ? abc->get_third().y - abc->get_second().y : diff;
		auto alpha = i * 1.0f / total_height;
		auto beta = (i - (second_half ? diff : 0)) * 1.0f / segment_height;

		auto A = Point2D{ abc->get_first().x + (int)(alpha * (abc->get_third().x - abc->get_first().x)),
						abc->get_first().y + (int)(alpha * (abc->get_third().y - abc->get_first().y)) };

		auto B = second_half ? Point2D{ abc->get_second().x + (int)(beta * (abc->get_third().x - abc->get_second().x)),
									abc->get_second().y + (int)(beta * (abc->get_third().y - abc->get_second().y)) } :
			Point2D{ abc->get_first().x + (int)(beta * (abc->get_second().x - abc->get_first().x)),
				abc->get_first().y + (int)(beta * (abc->get_second().y - abc->get_first().y)) };
		if (A.x > B.x)
		{
			auto temp = A;
			A = B;
			B = temp;
		}
		for (int j = A.x; j <= B.x; j++)
		{
			SDL_RenderDrawPoint(_sdl_render.get(), j, abc->get_first().y + i);
		}
	}
}

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

Point2D Triangle::get_first()
{
	return this->first;
}

Point2D Triangle::get_second()
{
	return this->second;
}

Point2D Triangle::get_third()
{
	return this->third;
}

void Triangle::set_first(Point2D a)
{
	this->first = a;
}

void Triangle::set_second(Point2D a)
{
	this->second = a;
}

void Triangle::set_third(Point2D a)
{
	this->third = a;
}