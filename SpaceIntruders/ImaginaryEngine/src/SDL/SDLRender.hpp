#include <render.hpp>
#include <SDL.h>

struct Point2D
{
	int x;
	int y;
};

class Engine;
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

class SDLRender final : public Render
{
public:
    explicit SDLRender(const Engine& engine, std::shared_ptr<SDL_Window> sdlWindow);
    void draw() override;
    void drawLineByPoints(int, int, int, int);
	void drawLineByPoints(Point2D, Point2D);
	void fillTriangle(Triangle*);
private:
    std::shared_ptr<SDL_Window> _sdlWindow;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> _sdlRenderer;
    const Engine& _engine;
};