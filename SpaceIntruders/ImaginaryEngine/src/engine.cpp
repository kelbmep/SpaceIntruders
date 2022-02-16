#include <engine.hpp>
#include <node.hpp>
#include <SDL/SDLWindow.hpp>
#include <audioManager.hpp>
#include <UIManager.hpp>
#include <playlist.hpp>
#include <fileManager.hpp>
#include <scheduleManager.hpp>

Engine::Engine()
{
    _file_manager = std::make_unique<FileManager>();
    _schedule_manager = std::make_unique<ScheduleManager>();
    /*_schedule_manager->schedule_update([this](fseconds delta)
        {
        _action_manager->update(delta);
        }, utils::gen_unique_object_ID());*/
}

void Engine::init(std::string window_name, size_t width, size_t height, int m)
{
    auto mode = ((m == 0) ? SDLWindow::RenderMode::OpenGL : SDLWindow::RenderMode::Software);

    _window = std::make_unique<SDLWindow>(*this, window_name.data(), width, height, mode);
    
    _m_world = new b2World(b2Vec2(0.0f, 6.8f));
    _virtual_resolution = glm::vec2(_window->get_width(), _window->get_height());

    _render = _window->create_render(); 
    _scene = std::make_shared<Node>(*this);
    _audio_manager = std::make_unique<AudioManager>(*this);
    
    _camera = std::make_shared<Node>(*this);
    _camera->set_size(_virtual_resolution);
    
    _event_manager = std::make_unique<EventManager>();
    _event_manager->add_delegate(this);
    
    _ui = std::make_unique<UIManager>(*this);
    _playlist = std::make_unique<Playlist>(*this);
    
    _render->init();
    
    _is_active = true;
}

bool Engine::is_active()
{
    return _is_active;
}

void Engine::update()
{
    _m_world->Step(1.0f / 120.0f, 8, 3);

    _audio_manager->update();
    _window->update();
    _schedule_manager->update();

    _scene->visit();
    _ui->visit();

    _render->draw();
    _window->swap();
    
    _playlist->update();
}

b2World& Engine::get_world() const
{
    return *_m_world;
}

const Window& Engine::get_window() const
{
    return *_window;
}

const Render& Engine::get_render() const
{
    return *_render;
}

const EventManager& Engine::get_event_manager() const
{
    return *_event_manager;
}

const AudioManager& Engine::get_audio_manager() const
{
    return *_audio_manager;
}

const ScheduleManager& Engine::get_schedule_manager() const
{
    return *_schedule_manager;
}

const FileManager& Engine::get_file_manager() const
{
    return *_file_manager;
}

std::shared_ptr<UIManager> Engine::get_ui_manager() const
{
    return _ui;
}

std::shared_ptr<Node> Engine::get_scene() const
{
    return _scene;
}

std::shared_ptr<Playlist> Engine::get_playlist() const
{
    return _playlist;
}

glm::vec2 Engine::get_virtual_resolution() const
{
    return _virtual_resolution;
}

void Engine::set_virtual_resolution(glm::vec2 virtual_resolution) const
{
    _virtual_resolution = virtual_resolution;
    _camera->set_size(_virtual_resolution);
}

std::shared_ptr<Node> Engine::get_camera() const
{
    return _camera;
}

void Engine::handle_event(QuitEvent)
{
    _is_active = false;
}

void Engine::handle_event(KeyEvent e)
{}

size_t Engine::get_window_width() const
{
	return _window->get_width();
}

size_t Engine::get_window_height() const
{
	return _window->get_height();
}

bool Engine::is_disabled() const
{
    return _is_disabled;
}

void Engine::enable()
{
    _is_disabled = false;
}

Engine::~Engine()
{
    _m_world->~b2World();
}