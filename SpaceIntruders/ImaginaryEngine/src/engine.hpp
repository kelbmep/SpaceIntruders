#pragma once

#include <eventManager.hpp>
#include <string>
#include <memory>

class Window;
class Render;
class Node;
class AudioManager;
class UIManager;

class Engine : public EventManager::Delegate 
{
public:
    explicit Engine();
    ~Engine();

    void init(std::string, size_t, size_t, int);
    void update();
    bool isActive();
    const Render& get_render() const;
    const Window& get_window() const;
    const EventManager& get_event_manager() const;
    const AudioManager& get_audio_manager() const;
	const UIManager& get_ui_manager() const;

    std::shared_ptr<Node> get_scene() const;

    void handle_event(EventManager::QuitEvent);
	void handle_event(EventManager::KeyEvent);
	
	size_t get_window_width() const;
    size_t get_window_height() const;
private:
	std::unique_ptr<Window> _window;
	std::unique_ptr<EventManager> _event_manager;
	std::unique_ptr<AudioManager> _audio_manager;
	std::unique_ptr<Render> _render;
	std::unique_ptr<UIManager> _ui;
	bool _isActive = false;

    std::shared_ptr<Node> _scene;
};