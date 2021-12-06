#pragma once

#include <render.hpp>
#include <SDL.h>

class SDL_Window;
class Engine;
class GLRender final : public Render
{
public:
	explicit GLRender(const Engine& engine, SDL_Window* window);

	void draw() override;

	std::shared_ptr<VertexBuffer> create_vertex_buffer(MeshData) const override;
	std::shared_ptr<ShaderProgram> create_program(std::string) const override;
	std::shared_ptr<Texture> create_texture(Bitmap) const override;
private:
	const Engine& _engine;
	std::unique_ptr<void, void(*)(void*)> _drawContext;
};
/*
#include <render.hpp>
#include <SDL.h>
#include <GL/glew.h>

class SDL_Window;

class GLRender : public Render
{
public:
	explicit GLRender(const Engine& engine, SDL_Window* window);
	void draw() override;
	void LoadShaderFromFile(std::string, GLuint&);
private:
	GLuint _VAO, _program;
	int _location;
	int _uScreenSize;
	const Engine& _engine;
	std::unique_ptr<void, void(*)(void*)> _drawContext;
};*/