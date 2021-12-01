#pragma once

#include <memory>

class Render;
class Window
{
public:
	virtual ~Window() = default;
	Window(size_t width, size_t height) :
		_width(width), _height(height)
	{}

	size_t getWidth() const
	{
		return _width;
	}
	size_t getHeight() const
	{
		return _height;
	}
	virtual void update() = 0;
	virtual void swap() = 0;
	virtual std::unique_ptr<Render> createRender() = 0;
private:
	size_t _width;
	size_t _height;
};