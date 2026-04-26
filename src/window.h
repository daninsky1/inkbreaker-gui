/*
 * Created by Daniel S. S. on 19/07/2025.
 */

#pragma once

#include "widget.h"

#include <SDL3/SDL.h>

namespace ui {

class Window : public SingleChildWidget
{
public:
    Window(std::string title, int32_t w, int32_t h, SDL_WindowFlags flags);
    ~Window() override = default;

    void update() const;

    Event& eventHandler(Event& event) override;
    Size layout(const BoxConstraints& constraint) override { return _size; };
    void render(BLContext& context, Position offset) override;
protected:
    std::string _title;
    SDL_WindowFlags _flags;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _sdlRenderer = nullptr;
    SDL_Surface* _sdlSurface = nullptr;
    BLImage _image;
    BLImageData _imageData{};
    BLRgba32 _color = Colors::GRAY;
    SDL_Texture* _texture{};
    void setRenderSurface();
};

} // ui
