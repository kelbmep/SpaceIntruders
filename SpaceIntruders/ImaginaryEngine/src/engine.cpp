#define GLEW_STATIC
#include <windows.h>
#include <SDL_version.h>
#include <SDL.h>
#include <GL/glew.h>
#include <SDL/SDLWindow.hpp>
#include <eventManager.hpp>
#include <window.hpp>
#include <render.hpp>
#include "engine.hpp"

Engine::Engine() 
{
	_eventManager = std::make_unique<EventManager>();
	_eventManager->add_delegate(this);
}

Engine::~Engine() = default;

bool Engine::isActive() 
{
	return _isActive;
}

void Engine::init(std::string name_window, size_t width, size_t height, int m)
{
	SDLWindow::RenderMode mode = ((m == 0) ? SDLWindow::RenderMode::OpenGL : SDLWindow::RenderMode::Software);

	_isActive = true;
	_window = std::make_unique<SDLWindow>(*this, name_window.data(), width, height, mode);
	_render = _window->createRender();
}

void Engine::update() 
{
	_window->update();
	_render->draw();
	_window->swap();
}

const EventManager& Engine::get_event_manager() const
{
	return *_eventManager;
}

void Engine::handle_event(EventManager::QuitEvent ev)
{
	_isActive = false;
}

void Engine::drawObj(std::vector<VertexTriangle> model)
{
	_render->load_picture(model);
}

size_t Engine::get_window_width() const
{
	return _window->getWidth();
}

size_t Engine::get_window_height() const
{
	return _window->getHeight();
}