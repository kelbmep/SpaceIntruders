#include <engine.hpp>
#include <node.hpp>
#include <SDL/SDLWindow.hpp>
#include <audioManager.hpp>
#include <UIManager.hpp>

Engine::Engine()
{

}

void Engine::init(std::string window_name, size_t width, size_t height, int m)
{
    auto mode = ((m == 0) ? SDLWindow::RenderMode::OpenGL : SDLWindow::RenderMode::Software);

    _window = std::make_unique<SDLWindow>(*this, window_name.data(), width, height, mode);
    _render = _window->create_render(); 
    _scene = std::make_shared<Node>();
    _isActive = true;
    _audio_manager = std::make_unique<AudioManager>();
    
    _event_manager = std::make_unique<EventManager>();
    _event_manager->add_delegate(this);
    
    _ui = std::make_unique<UIManager>(*this);
}

bool Engine::isActive()
{
    return _isActive;
}

void Engine::update()
{
    _audio_manager->update();
    _window->update();
    
    _scene->visit();
    _ui->visit();

    _render->draw();
    _window->swap();
}

const Render& Engine::get_render() const
{
    return *_render;
}

const Window& Engine::get_window() const
{
    return *_window;
}

const EventManager& Engine::get_event_manager() const
{
    return *_event_manager;
}

const AudioManager& Engine::get_audio_manager() const
{
    return *_audio_manager;
}

const UIManager& Engine::get_ui_manager() const
{
    return *_ui;
}

std::shared_ptr<Node> Engine::get_scene() const
{
    return _scene;
}

Engine::~Engine() = default;

void Engine::handle_event(EventManager::QuitEvent ev)
{
    _isActive = false;
}

void Engine::handle_event(EventManager::KeyEvent ev)
{

}

size_t Engine::get_window_width() const
{
	return _window->get_width();
}

size_t Engine::get_window_height() const
{
	return _window->get_height();
}