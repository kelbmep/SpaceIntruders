#include <cstdio>
#include <string>
#include <memory>
#include <iostream>

class Point2D {
public:
	explicit Point2D();
	Point2D(int);
	Point2D(int, int);
	int getX();
	int getY();
	void setX(int);
	void setY(int);
private:
	int x;
	int y;
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
	void drawLineByPoints(int, int, int, int);
	void drawLineByPoints(Point2D*, Point2D*);
private:
	struct Pimpl;
	std::unique_ptr<Pimpl> _pimpl;
};