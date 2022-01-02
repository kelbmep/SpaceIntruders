#pragma once

#include <eventManager.hpp>
#include <string>
#include <memory>

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
    
    const Render& get_render() const;
    const Window& get_window() const;
    const EventManager& get_event_manager() const;
    const AudioManager& get_audio_manager() const;
	
    std::shared_ptr<UIManager> get_ui_manager() const;
    std::shared_ptr<Playlist> get_playlist() const;
    std::shared_ptr<Node> get_scene() const;

    void handle_event(QuitEvent) override;
    void handle_event(KeyEvent) override;

	size_t get_window_width() const;
    size_t get_window_height() const;
private:
    std::unique_ptr<Render> _render;
    std::unique_ptr<Window> _window;
	std::unique_ptr<EventManager> _event_manager;
	std::unique_ptr<AudioManager> _audio_manager;
	
    std::shared_ptr<UIManager> _ui;
    std::shared_ptr<Playlist> _playlist;
    std::shared_ptr<Node> _scene;

    bool _is_active = false;
};