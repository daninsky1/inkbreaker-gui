/*
* Created by Daniel S. S. on 19/07/2025.
*/

#include "window.h"

namespace ui {
Window::Window(std::string title, int32_t w, int32_t h, SDL_WindowFlags flags)
    : _title(std::move(title)), _flags(flags)
{
    _size = { w, h };
    _boxConstraints = BoxConstraints{w, h, w, h};
    if (!SDL_CreateWindowAndRenderer(_title.c_str(), w, h, _flags, &_window, &_sdlRenderer)) {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
    }
    SDL_SetWindowBordered(_window, true);
    setRenderSurface();
}

Window::~Window()
{
    if (_renderer != nullptr) {
        _renderer->releaseRenderTarget();
        delete _renderer;
        _renderer = nullptr;
    }
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
    if (_sdlRenderer != nullptr) {
        SDL_DestroyRenderer(_sdlRenderer);
        _sdlRenderer = nullptr;
    }
    if (_window != nullptr) {
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

void Window::setRenderSurface()
{
    int w, h;
    if (!SDL_GetWindowSize(_window, &w, &h)) {
        SDL_Log("Failed get window size: %s", SDL_GetError());
        return;
    }

    gfx::ImageInfo info{.dimension = {w, h}};
    _surface = gfx::Surface::create(info);

    if (_renderer == nullptr) {
        _renderer = gfx::Renderer::create();
    } else {
        _renderer->releaseRenderTarget();
    }
    _renderer->bindRenderTarget(_surface);
    _renderer->clear(_color);

    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }

    _texture = SDL_CreateTexture(
        _sdlRenderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        w, h
    );
    if (_texture == nullptr) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
    }
}

void Window::setMinimumSize(int32_t width, int32_t height)
{
    if (!SDL_SetWindowMinimumSize(_window, width, height)) {
        SDL_Log("Failed to set window minimum size: %s", SDL_GetError());
    }
}

void Window::render(gfx::Renderer* renderer, Position offset)
{
    (void)renderer;
    (void)offset;

    if (_child == nullptr) {
        SDL_Log("No child widget to render.");
        return;
    }

    _renderer->clear(_color);
    _child->render(_renderer, {0, 0});

    if (!SDL_UpdateTexture(_texture, nullptr, _surface->getData(), _surface->getPitch())) {
        SDL_Log("Failed to update texture: %s", SDL_GetError());
        return;
    }

    SDL_RenderClear(_sdlRenderer);
    SDL_RenderTexture(_sdlRenderer, _texture, nullptr, nullptr);
    SDL_RenderPresent(_sdlRenderer);
}

void Window::update() const {
    if (_child != nullptr) {
        _child->layout(_boxConstraints);
    }
}

Event& Window::eventHandler(Event& event)
{
    // Handle events specific to the window, e.g., resizing, closing, etc.
    SDL_Event sdlEvent = event.sdlEvent;
    switch (sdlEvent.type) {
    case SDL_EVENT_MOUSE_MOTION:
        event.hasPosition = true;
        event.position = {
            .x = static_cast<int32_t>(sdlEvent.motion.x),
            .y = static_cast<int32_t>(sdlEvent.motion.y)
        };
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
        event.hasPosition = true;
        event.position = {
            .x = static_cast<int32_t>(sdlEvent.button.x),
            .y = static_cast<int32_t>(sdlEvent.button.y)
        };
        break;
    case SDL_EVENT_WINDOW_RESIZED: {
        int w = sdlEvent.window.data1;
        int h = sdlEvent.window.data2;
        SDL_Log("Window resized to %d x %d", w, h);
        _size = Size(w, h);
        _boxConstraints = BoxConstraints::tight(_size);
        setRenderSurface();
        event.handled = true; // Mark the event as handled
        return event;
    }
    }
    
    // Call the parent class's event handler
    return SingleChildWidget::eventHandler(event);
}

} // ui 
