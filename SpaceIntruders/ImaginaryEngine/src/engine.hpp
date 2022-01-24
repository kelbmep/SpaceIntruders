#pragma once

#include <eventManager.hpp>
#include <memory>
#include <glm/glm.hpp>
#include <box2d/box2d.h>

class Window;
class Render;
class Node;
class AudioManager;
class UIManager;
class Playlist;

class Engine : public EventManager::Delegate 
{
public:
    explicit Engine();
    ~Engine();

    void init(std::string, size_t, size_t, int);
    
    void update();
    bool is_active();
    
    b2World& get_world() const;

    const Render& get_render() const;
    const Window& get_window() const;
    const EventManager& get_event_manager() const;
    const AudioManager& get_audio_manager() const;
	
    std::shared_ptr<UIManager> get_ui_manager() const;
    std::shared_ptr<Playlist> get_playlist() const;
    std::shared_ptr<Node> get_scene() const;
    std::shared_ptr<Node> get_camera() const;

    glm::vec2 get_virtual_resolution() const;
    void set_virtual_resolution(glm::vec2) const;

    void handle_event(QuitEvent) override;
    void handle_event(KeyEvent) override;

	size_t get_window_width() const;
    size_t get_window_height() const;
private:
    b2World* _m_world;

    std::unique_ptr<Render> _render;
    std::unique_ptr<Window> _window;
	std::unique_ptr<EventManager> _event_manager;
	std::unique_ptr<AudioManager> _audio_manager;
    
    std::shared_ptr<UIManager> _ui;
    std::shared_ptr<Playlist> _playlist;
    std::shared_ptr<Node> _scene;
    std::shared_ptr<Node> _camera;

    mutable glm::vec2 _virtual_resolution;

    bool _is_active = false;
};