/*
 * Created by Daniel S. S. on 19/07/2025.
 */

#pragma once

#include "widget.h"

#include <SDL3/SDL.h>
#include <memory>

namespace ui {

class Window : public SingleChildWidget
{
public:
    Window(std::string title, int32_t w, int32_t h, SDL_WindowFlags flags);
    ~Window() override;

    void setMinimumSize(int32_t width, int32_t height);
    void update() const;

    Event& eventHandler(Event& event) override;
    Size layout(const BoxConstraints& constraint) override { return _size; };
    void render(gfx::Renderer* renderer, Position offset) override;
protected:
    std::string _title;
    SDL_WindowFlags _flags;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _sdlRenderer = nullptr;
    SDL_Surface* _sdlSurface = nullptr;
    std::shared_ptr<gfx::Surface> _surface;
    gfx::Renderer* _renderer = nullptr;
    gfx::Color _color = gfx::Colors::GRAY;
    SDL_Texture* _texture{};
    void setRenderSurface();
};

} // ui
