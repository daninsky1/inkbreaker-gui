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

void Window::setRenderSurface()
{
    int w, h;
    if (!SDL_GetWindowSize(_window, &w, &h)) {
        SDL_Log("Failed get window size: %s", SDL_GetError());
        return;
    }

    _image = BLImage(w, h, BL_FORMAT_PRGB32);
    _image.getData(&_imageData);

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

void Window::render(BLContext& context, Position offset)
{
    (void)context;
    (void)offset;

    if (_child == nullptr) {
        SDL_Log("No child widget to render.");
        return;
    }

    BLContext childContext(_image);
    childContext.fillAll(_color);
    _child->render(childContext, {0, 0});
    childContext.end();

    _image.getData(&_imageData);

    if (!SDL_UpdateTexture(_texture, nullptr, _imageData.pixelData, static_cast<int>(_imageData.stride))) {
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
    case SDL_EVENT_WINDOW_RESIZED: {
        int w = sdlEvent.window.data1;
        int h = sdlEvent.window.data2;
        SDL_Log("Window resized to %d x %d", w, h);
        _size = Size(w, h);
        _boxConstraints = BoxConstraints(w, h, w, h);
        setRenderSurface();
        event.handled = true; // Mark the event as handled
        return event;
    }
    }
    
    // Call the parent class's event handler
    return SingleChildWidget::eventHandler(event);
}

} // ui 
