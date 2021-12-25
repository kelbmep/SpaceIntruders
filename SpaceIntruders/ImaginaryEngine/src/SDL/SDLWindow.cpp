#include <SDL/SDLWindow.hpp>
#include <GL/GLRender.hpp>
#include <SDL/SDLRender.hpp>

SDLWindow::SDLWindow(const Engine& engine, std::string name, size_t width, size_t height, RenderMode renderMode)
    : Window(width, height), _engine(engine), _renderMode(renderMode)
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
                             (int)width,
                             (int)height,
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
    else if (e.type == SDL_KEYDOWN || e.type == SDL_KEYUP)
    {
        auto type = EventManager::KeyType::KeyUp;
        if(e.type == SDL_KEYDOWN)
            type = EventManager::KeyType::KeyDown;
        auto code = EventManager::KeyCode::Unknown;

        switch (e.key.keysym.sym)
        {
        case SDLK_UP:
            code = EventManager::KeyCode::Up;
            break;
        case SDLK_DOWN:
            code = EventManager::KeyCode::Down;
            break;
        case SDLK_LEFT:
            code = EventManager::KeyCode::Left;
            break;
        case SDLK_RIGHT:
            code = EventManager::KeyCode::Right;
            break;
        case SDLK_a:
            code = EventManager::KeyCode::A;
            break;
        case SDLK_d:
            code = EventManager::KeyCode::D;
            break;
        case SDLK_SPACE:
            code = EventManager::KeyCode::Space;
            break;
        case SDLK_ESCAPE:
            event_manager.invoke_event(EventManager::QuitEvent{});
            break;
        default: break;
        }
        event_manager.invoke_event(EventManager::KeyEvent{ code, type });
    }
    else if (e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
    {
        auto type = EventManager::KeyType::KeyUp;
        if (e.type == SDL_MOUSEBUTTONDOWN)
            type = EventManager::KeyType::KeyDown;

        EventManager::MouseButton button;

        switch (e.button.button)
        {
        case SDL_BUTTON_LEFT:
            button = EventManager::MouseButton::Left;
            break;
        case SDL_BUTTON_RIGHT:
            button = EventManager::MouseButton::Right;
            break;
        case SDL_BUTTON_MIDDLE:
            button = EventManager::MouseButton::Middle;
            break;
        default:
            break;
        }
        event_manager.invoke_event(EventManager::MouseEvent{ e.button.x, e.button.y, type, button});
    }
    else if (e.type == SDL_MOUSEMOTION)
    {
        event_manager.invoke_event(EventManager::MouseMoveEvent{ e.button.x, e.button.y });
    }
    else if (e.type == SDL_TEXTINPUT)
    {
        event_manager.invoke_event(EventManager::TextInputEvent{ e.text.text });
    }
}

std::unique_ptr<Render> SDLWindow::create_render()
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
