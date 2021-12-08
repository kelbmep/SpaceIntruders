#include <SDL/SDLRender.hpp>

SDLRender::SDLRender(const Engine& engine, std::shared_ptr<SDL_Window> sdlWindow)
	: _engine{ engine }
	, _sdlWindow(std::move(sdlWindow))
	, _sdlRender{ SDL_CreateRenderer(_sdlWindow.get(), -1, 0), SDL_DestroyRenderer }
{
}

void SDLRender::draw()
{
	int screen_width = static_cast<int>(_engine.get_window_width());
	int screen_height = static_cast<int>(_engine.get_window_height());
	int difference = 0;
	if (screen_width > screen_height)
	{
		difference = screen_width - screen_height;
	}
	int objWidth = screen_width - difference;
	int objHeight = screen_height;

	/*for (auto& tr : _picture)
	{
		Point2D a = { static_cast<int>(difference / 2 + (tr.arr[0].x + 1.) * objWidth / 2.), objHeight - static_cast<int>((tr.arr[0].y + 1.) * objHeight / 2.) };
		Point2D b = { static_cast<int>(difference / 2 + (tr.arr[1].x + 1.) * objWidth / 2.), objHeight - static_cast<int>((tr.arr[1].y + 1.) * objHeight / 2.) };
		Point2D c = { static_cast<int>(difference / 2 + (tr.arr[2].x + 1.) * objWidth / 2.), objHeight - static_cast<int>((tr.arr[2].y + 1.) * objHeight / 2.) };
		Triangle abc(a, b, c);
		fillTriangle(&abc);
	}*/
	SDL_RenderPresent(_sdlRender.get());

	//_commands.clear();
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

void SDLRender::drawLineByPoints(int x0, int y0, int x1, int y1)
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
			SDL_RenderDrawPoint(_sdlRender.get(), y, x);
		}
		else
		{
			SDL_RenderDrawPoint(_sdlRender.get(), x, y);
		}
		error2 += derror2;

		if (error2 > dx)
		{
			y += (y1 > y0 ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void SDLRender::drawLineByPoints(Point2D a, Point2D b)
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
			SDL_RenderDrawPoint(_sdlRender.get(), y, x);
		}
		else
		{
			SDL_RenderDrawPoint(_sdlRender.get(), x, y);
		}
		error2 += derror2;

		if (error2 > dx)
		{
			y += (b.y > a.y ? 1 : -1);
			error2 -= dx * 2;
		}
	}
}

void SDLRender::fillTriangle(Triangle* abc)
{
	SDL_SetRenderDrawColor(_sdlRender.get(), rand() % 256, rand() % 256, rand() % 256, rand() % 256);
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

		Point2D A = Point2D{ abc->getFirst().x + (int)(alpha * (abc->getThird().x - abc->getFirst().x)),
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
			SDL_RenderDrawPoint(_sdlRender.get(), j, abc->getFirst().y + i);
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