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

	SDLWindow(const Engine&,
			std::string,
			size_t,
			size_t,
			RenderMode);

	void update() override;
	void swap() override;
	std::unique_ptr<Render> create_render() override;
private:
	std::shared_ptr<SDL_Window> _window;
	RenderMode _renderMode;
	const Engine& _engine;
};