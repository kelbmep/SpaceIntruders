#pragma once

#include <string>
#include <memory>
#include <iostream>
#include <fstream>
#include <eventManager.hpp>

class Window;
class Render;

class Engine : public EventManager::Delegate {
public:
	explicit Engine();
	struct Vertex
	{
		float x;
		float y;
		float r;
		float g;
		float b;
	};

	struct VertexTriangle
	{
		Vertex arr[3];
	};

	~Engine();
	void init(std::string, size_t, size_t, int);
	void update();
	bool isActive();
	const EventManager& get_event_manager() const;
	void handle_event(EventManager::QuitEvent ev);
	size_t get_window_width() const;
	size_t get_window_height() const;
	void drawObj(std::vector<VertexTriangle>);
private:
	bool _isActive = false;
	std::unique_ptr<Window> _window;
	std::unique_ptr<EventManager> _eventManager;
	std::unique_ptr<Render> _render;
};