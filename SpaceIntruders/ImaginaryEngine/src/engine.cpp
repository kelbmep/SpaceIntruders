#define GLEW_STATIC

#include <engine.hpp>
#include <node.hpp>
#include <memory>
#include <SDL/SDLWindow.hpp>
#include <SDL/SDLRender.hpp>
#include <eventManager.hpp>

Engine::Engine(std::shared_ptr<EventManager> ea)
{
    _eventManager = ea;// std::make_unique<EventManager>();
    _eventManager->add_delegate(this);
    //_eventManager = createUnique<EventsManager>();
}

void Engine::init(std::string window_name, size_t width, size_t height, int m)
{
    SDLWindow::RenderMode mode = ((m == 0) ? SDLWindow::RenderMode::OpenGL : SDLWindow::RenderMode::Software);

    _window = std::make_unique<SDLWindow>(*this, window_name.data(), width, height, mode);
    _render = _window->create_render(); 
    _scene = std::make_shared<Node>();
    _isActive = true;

    /*EventsAutoRegistarator reg(*_eventsManager, EventsAutoRegistarator::NoRemove);
    reg += [this](const SystemEvent& e)
    {
        if (e.type == SystemEvent::Type::Quit)
        {
            _isActive = false;
        }
    };*/
}

bool Engine::isActive()
{
    return _isActive;
}

void Engine::update()
{
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

/*void Engine::handle_event(EventManager::KeyDownEvent ev)
{
    _scene->set_position(glm::vec2(_scene->get_position().x, _scene->get_position().y + 2));
}

void Engine::handle_event(EventManager::KeyUpEvent ev)
{
    _scene->set_position(glm::vec2(_scene->get_position().x, _scene->get_position().y - 2));
}

void Engine::handle_event(EventManager::KeyLeftEvent ev)
{
    _scene->set_position(glm::vec2(_scene->get_position().x - 2, _scene->get_position().y));
}

void Engine::handle_event(EventManager::KeyRightEvent ev)
{
    _scene->set_position(glm::vec2(_scene->get_position().x + 2, _scene->get_position().y));
}

void Engine::handle_event(EventManager::KeyAEvent ev)
{
}

void Engine::handle_event(EventManager::KeyDEvent ev)
{
}*/
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