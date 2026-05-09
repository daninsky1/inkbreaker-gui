/*
 * Created by Daniel S. S. on 19/07/2025.
 */

#pragma once

#include "widget.h"

#include <memory>

#include <SDL3/SDL.h>

namespace ui {

class RenderBackend;

class Window : public SingleChildWidget
{
public:
    Window(std::string title, int32_t w, int32_t h, SDL_WindowFlags flags);
    ~Window() override;

    void setMinimumSize(int32_t width, int32_t height);
    void update();

    Event& eventHandler(Event& event) override;
    Size layout(const BoxConstraints& constraint) override { return _size; };
    void render(Position offset) override;
protected:
    std::string _title;
    SDL_WindowFlags _flags;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _sdlRenderer = nullptr;
    SDL_Surface* _sdlSurface = nullptr;
    Color _color = Colors::GRAY;
    SDL_Texture* _texture{};
    std::unique_ptr<RenderBackend> _renderBackend;
    void setRenderSurface();
};

} // ui
