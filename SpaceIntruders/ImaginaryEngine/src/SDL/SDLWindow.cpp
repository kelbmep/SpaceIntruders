#define GLEW_STATIC
#include <GL/glew.h>
#include <Engine.hpp>
#include <EventManager.hpp>
#include <SDL/SDLRender.hpp>
#include <GL/GLRender.hpp>
#include "SDLWindow.hpp"

SDLWindow::SDLWindow(const Engine& engine,
				std::string name,
				size_t width,
				size_t height,
				RenderMode renderMode) :
	Window(width, height),
	_engine(engine),
	_renderMode(renderMode)
{
    SDL_version version = { 0, 0, 0 };

    SDL_GetVersion(&version);

    printf("SDL Version: %d.%d.%d\n", version.major, version.minor, version.patch);

    SDL_Init(SDL_INIT_EVERYTHING);

    Uint32 windowFlags = SDL_WINDOW_SHOWN;

    if (_renderMode == RenderMode::OpenGL)
    {
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

        windowFlags |= SDL_WINDOW_OPENGL;
    }

    _window = { SDL_CreateWindow(name.data(),
                             SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                             static_cast<int>(width),
                             static_cast<int>(height),
                             windowFlags), SDL_DestroyWindow };
}

void SDLWindow::update()
{
    SDL_Event e;
    const auto& event_manager = _engine.get_event_manager();

    SDL_PollEvent(&e);
    if (e.type == SDL_QUIT) 
    {
         event_manager.invoke_event(EventManager::QuitEvent{});
    }
}

std::unique_ptr<Render> SDLWindow::createRender()
{
    if (_renderMode == RenderMode::OpenGL)
    {
        return std::make_unique<GLRender>(_engine, _window.get());
    }
    else 
    {
        return std::make_unique<SDLRender>(_engine, _window);
    }
}

void SDLWindow::swap()
{
    if (_renderMode == RenderMode::OpenGL)
    {
        SDL_GL_SwapWindow(_window.get());
    }
}