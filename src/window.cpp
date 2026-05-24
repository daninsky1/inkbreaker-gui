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
}

Window::~Window()
{
}

void Window::setMinimumSize(int32_t width, int32_t height)
{
    _minimumSize = {width, height};
    if (_window == nullptr) {
        return;
    }

    if (!SDL_SetWindowMinimumSize(_window, width, height)) {
        SDL_Log("Failed to set window minimum size: %s", SDL_GetError());
    }
}

void Window::setTitle(std::string title)
{
    _title = std::move(title);
    if (_window == nullptr) {
        return;
    }

    if (!SDL_SetWindowTitle(_window, _title.c_str())) {
        SDL_Log("Failed to set window title: %s", SDL_GetError());
    }
}

void Window::render(gfx::Renderer* renderer, Position offset)
{
    (void)offset;

    if (renderer == nullptr) {
        SDL_Log("No renderer configured for window render.");
        return;
    }

    if (_child == nullptr) {
        SDL_Log("No child widget to render.");
        return;
    }

    renderer->clear(_color);
    _child->render(renderer, {0, 0});
}

Size Window::layout(const BoxConstraints& constraint)
{
    (void)constraint;

    if (_child != nullptr) {
        _child->layout(_boxConstraints);
    }
    return _size;
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
        event.handled = true; // Mark the event as handled
        return event;
    }
    }
    
    // Call the parent class's event handler
    return SingleChildWidget::eventHandler(event);
}

void Window::attachNativeWindow(SDL_Window* window)
{
    _window = window;
    if (_window == nullptr) {
        return;
    }

    if (!_title.empty() && !SDL_SetWindowTitle(_window, _title.c_str())) {
        SDL_Log("Failed to set window title: %s", SDL_GetError());
    }
    if (_minimumSize.hasSize() && !SDL_SetWindowMinimumSize(_window, _minimumSize.width, _minimumSize.height)) {
        SDL_Log("Failed to set window minimum size: %s", SDL_GetError());
    }
}

void Window::detachNativeWindow(SDL_Window* window)
{
    if (_window == window) {
        _window = nullptr;
    }
}

} // ui 
