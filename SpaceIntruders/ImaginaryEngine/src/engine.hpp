#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <eventManager.hpp>
#include <audioManager.hpp>

class Window;
class Render;
//class EventManager;
//class VertexBuffer;
class Node;

class Engine : public EventManager::Delegate 
{
public:
    explicit Engine(std::shared_ptr<EventManager>, std::shared_ptr<AudioManager>);
    ~Engine();

    void init(std::string, size_t, size_t, int);
    void update();
    bool isActive();
    const EventManager& get_event_manager() const;
    const Render& get_render() const;
    const Window& get_window() const;
    
    std::shared_ptr<Node> get_scene();

    void handle_event(EventManager::QuitEvent);
	void handle_event(EventManager::KeyEvent);
	/*void handle_event(EventManager::KeyUpEvent);
	void handle_event(EventManager::KeyDownEvent);
	void handle_event(EventManager::KeyLeftEvent);
	void handle_event(EventManager::KeyRightEvent);
	void handle_event(EventManager::KeyAEvent);
	void handle_event(EventManager::KeyDEvent);*/
	
	size_t get_window_width() const;
    size_t get_window_height() const;
    //void drawObj(std::vector<VertexTriangle>);
    
private:
	std::unique_ptr<Window> _window;
	std::shared_ptr<EventManager> _eventManager;
	std::shared_ptr<AudioManager> _audioManager;
	std::unique_ptr<Render> _render;
	bool _isActive = false;

    std::shared_ptr<Node> _scene;
};
	/*struct Vertex
	{
		float x;
		float y;
		float r;
		float g;
		float b;
		float t_x;
		float t_y;
	};

	struct VertexTriangle
	{
		Vertex arr[3];
	};*/