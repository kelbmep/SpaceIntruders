#pragma once

#include <string>
#include <memory>
#include <iostream>

struct Point2D
{
	int x;
	int y;
};

class Triangle {
public:
	explicit Triangle();
	Triangle(Point2D, Point2D, Point2D);
	Point2D getFirst();
	Point2D getSecond();
	Point2D getThird();
	void setFirst(Point2D);
	void setSecond(Point2D);
	void setThird(Point2D);
private:
	Point2D first;
	Point2D second;
	Point2D third;
};

class Engine {
public:
	explicit Engine();
	~Engine();
	void init();
	void init(std::string);
	void update();
	bool isActive();
	void drawObj(const std::string, int, int);
private:
	struct Pimpl;
	std::unique_ptr<Pimpl> _pimpl;
};