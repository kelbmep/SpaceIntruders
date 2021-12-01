#pragma once
#include <window.hpp>
#include <SDL.h>
#include <string>

class Engine;

class SDLWindow final : public Window
{
public:
	enum class RenderMode
	{
		OpenGL,
		Software
	};

	SDLWindow(const Engine& engine,
		std::string name,
		size_t width,
		size_t height,
		RenderMode renderMode);

	void update() override;
	void swap() override;
	std::unique_ptr<Render> createRender() override;
private:
	std::shared_ptr<SDL_Window> _window;
	RenderMode _renderMode;
	const Engine& _engine;
};
