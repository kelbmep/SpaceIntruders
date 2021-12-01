#pragma once

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
	int _uScreenSize;
	const Engine& _engine;
	std::unique_ptr<void, void(*)(void*)> _drawContext;
};