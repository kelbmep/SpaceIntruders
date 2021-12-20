#pragma once

#include <memory>

class Render;

class Window
{
public:
	virtual ~Window() = default;
	Window(size_t w, size_t h) :
		_width(w), _height(h)
	{}

	size_t get_width() const
	{
		return _width;
	}
	size_t get_height() const
	{
		return _height;
	}
	
	virtual void update() = 0;
	virtual void swap() = 0;
	virtual std::unique_ptr<Render> create_render() = 0;
private:
	size_t _width;
	size_t _height;
};