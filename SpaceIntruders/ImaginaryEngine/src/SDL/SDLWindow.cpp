#define GLEW_STATIC

#include <GL/glew.h>
#include <SDL/SDLWindow.hpp>
#include <SDL/SDLRender.hpp>
#include <GL/GLRender.hpp>
#include <engine.hpp>
#include <eventManager.hpp>
#include <memory>

SDLWindow::SDLWindow(const Engine& engine,
    std::string name,
    size_t width,
    size_t height,
    RenderMode renderMode)
    : Window(width, height)
    , _engine(engine)
    , _renderMode(renderMode)
{
    SDL_version version = { 0, 0, 0 };

    SDL_GetVersion(&version);

    printf("SDL Version: %d.%d.%d", version.major, version.minor, version.patch);

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
                             width,
                             height,
                             windowFlags), SDL_DestroyWindow };

    /*int w, h;
    SDL_GL_GetDrawableSize(_window.get(), &w, &h);

    _width = w;
    _height = h;
    */
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
    else if (e.type == SDL_KEYDOWN)
    {
        switch (e.key.keysym.sym)
        {
        case SDLK_DOWN:
            event_manager.invoke_event(EventManager::KeyDownEvent{});
            break;
        case SDLK_UP:
            event_manager.invoke_event(EventManager::KeyUpEvent{});
            break;
        case SDLK_LEFT:
            event_manager.invoke_event(EventManager::KeyLeftEvent{});
            break;
        case SDLK_RIGHT:
            event_manager.invoke_event(EventManager::KeyRightEvent{});
            break;
        //case SDLK_SPACE:
        //    event_manager.invoke_event(EventManager::KeySpaceEvent{});
        //    break;
        case SDLK_a:
            event_manager.invoke_event(EventManager::KeyAEvent{});
            break;
        case SDLK_d:
            event_manager.invoke_event(EventManager::KeyDEvent{});
            break;
        default: break;
        }
    }
}

std::unique_ptr<Render> SDLWindow::create_render()
{
    if (_renderMode == RenderMode::OpenGL)
    {
        return std::make_unique<GLRender>(_engine, _window.get());
        //return std::make_unique<SDLRender>(_engine, _window);
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
