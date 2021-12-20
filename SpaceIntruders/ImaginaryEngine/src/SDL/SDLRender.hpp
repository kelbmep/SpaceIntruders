#pragma once

#include <render.hpp>
#include <SDL.h>

struct Point2D
{
	int x;
	int y;
};

class Triangle {
public:
	explicit Triangle();
	Triangle(Point2D, Point2D, Point2D);
	Point2D get_first();
	Point2D get_second();
	Point2D get_third();
	void set_first(Point2D);
	void set_second(Point2D);
	void set_third(Point2D);
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
	void draw_line_by_points(int, int, int, int);
	void draw_line_by_points(Point2D, Point2D);
	void fill_triangle(Triangle*);

	std::shared_ptr<ShaderProgram> create_program(std::string) const override;
	std::shared_ptr<VertexBuffer> create_vertex_buffer(MeshData) const override;
	std::shared_ptr<Texture> create_texture(Bitmap) const override;
private:
	std::shared_ptr<SDL_Window> _sdl_window;
	std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> _sdl_render;
	const Engine& _engine;
};