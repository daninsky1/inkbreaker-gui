/*
 * Created by Daniel S. S. on 19/07/2025.
 */

#pragma once

#include "widget.h"

#include <SDL3/SDL.h>

namespace ui {

class SdlRenderHost;

class Window : public SingleChildWidget
{
public:
    Window(std::string title, int32_t w, int32_t h, SDL_WindowFlags flags);
    ~Window() override;

    void setMinimumSize(int32_t width, int32_t height);
    void setTitle(std::string title);
    const std::string& getTitle() const { return _title; }
    SDL_WindowFlags getFlags() const { return _flags; }
    Size getMinimumSize() const { return _minimumSize; }
    SDL_Window* nativeHandle() const { return _window; }
    bool getResizeRequest();

    Event& eventHandler(Event& event) override;
    Size layout(const BoxConstraints& constraint) override;
    void render(gfx::Renderer* renderer, Position offset) override;
protected:
    friend class SdlRenderHost;

    void attachNativeWindow(SDL_Window* window);
    void detachNativeWindow(SDL_Window* window);

    std::string _title;
    SDL_WindowFlags _flags;
    Size _minimumSize = {0, 0};
    SDL_Window* _window = nullptr;
    gfx::Color _color = gfx::Colors::GRAY;
};

} // ui
