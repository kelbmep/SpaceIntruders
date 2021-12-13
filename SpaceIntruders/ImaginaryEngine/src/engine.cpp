#define GLEW_STATIC

#include <engine.hpp>
#include <node.hpp>
#include <memory>
#include <SDL/SDLWindow.hpp>
#include <SDL/SDLRender.hpp>
#include <eventManager.hpp>

Engine::Engine(std::shared_ptr<EventManager> ea, std::shared_ptr<AudioManager> audioManager)
{
    _eventManager = ea;
    _eventManager->add_delegate(this);
    _audioManager = audioManager;
}

void Engine::init(std::string window_name, size_t width, size_t height, int m)
{
    SDLWindow::RenderMode mode = ((m == 0) ? SDLWindow::RenderMode::OpenGL : SDLWindow::RenderMode::Software);

    _window = std::make_unique<SDLWindow>(*this, window_name.data(), width, height, mode);
    _render = _window->create_render(); 
    _scene = std::make_shared<Node>();
    _isActive = true;
}

bool Engine::isActive()
{
    return _isActive;
}

void Engine::update()
{
    _audioManager->update();

    _window->update();
    
    _scene->visit();
    _render->draw();

    _window->swap();
}

const EventManager& Engine::get_event_manager() const
{
    return *_eventManager;
}

const Render& Engine::get_render() const
{
    return *_render;
}

std::shared_ptr<Node> Engine::get_scene()
{
    return _scene;
}

const Window& Engine::get_window() const
{
    return *_window;
}

Engine::~Engine() = default;

void Engine::handle_event(EventManager::QuitEvent ev)
{
    _isActive = false;
}

void Engine::handle_event(EventManager::KeyEvent ev)
{

}

/*void Engine::drawObj(std::vector<VertexTriangle> model)
{
	_render->load_picture(model);
}*/

size_t Engine::get_window_width() const
{
	return _window->get_width();
}

size_t Engine::get_window_height() const
{
	return _window->get_height();
}