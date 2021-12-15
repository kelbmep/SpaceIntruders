#pragma once

#include <render.hpp>
#include <SDL.h>
#include <bitmap.hpp>

struct Point2D
{
	int x;
	int y;
};

class Triangle {
public:
	explicit Triangle();
	Triangle(Point2D, Point2D, Point2D);
	Point2D getFirst();
	Point2D getSecond();
	Point2D getThird();
	void setFirst(Point2D);
	void setSecond(Point2D);
	void setThird(Point2D);
private:
	Point2D first;
	Point2D second;
	Point2D third;
};

class Engine;
class SDLRender final : public Render
{
public:
	explicit SDLRender(const Engine&, std::shared_ptr<SDL_Window>);

	void draw() override;
	void draw(size_t, size_t) override;
	void drawLineByPoints(int, int, int, int);
	void drawLineByPoints(Point2D, Point2D);
	void fillTriangle(Triangle*);

	std::shared_ptr<ShaderProgram> create_program(std::string) const override;
	std::shared_ptr<VertexBuffer> create_vertex_buffer(MeshData) const override;
	std::shared_ptr<Texture> create_texture(Bitmap) const override;
private:
	std::shared_ptr<SDL_Window> _sdlWindow;
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> _sdlRender;
	const Engine& _engine;
};