#pragma once

#include <memory>

#include <SDL3/SDL.h>

#include "layout.h"
#include "graphics/renderer.h"
#include "graphics/surface.h"

namespace ui {

class Window;

class SdlRenderHost
{
public:
    explicit SdlRenderHost(Window& window);
    ~SdlRenderHost();

    SdlRenderHost(const SdlRenderHost&) = delete;
    SdlRenderHost& operator=(const SdlRenderHost&) = delete;

    Size getSize() const;
    bool isReady() const;
    void resize(Size size);
    void renderFrame(Window& window);

private:
    void createRenderTarget(Size size);
    void destroyTexture();

    Window& _windowOwner;
    SDL_Window* _window = nullptr;
    SDL_Renderer* _sdlRenderer = nullptr;
    SDL_Texture* _texture = nullptr;
    std::shared_ptr<gfx::Surface> _surface;
    std::unique_ptr<gfx::Renderer> _renderer;
    Size _renderTargetSize = {0, 0};
};

} // namespace ui
