#include "sdl_render_host.h"

#include "window.h"

namespace ui {

SdlRenderHost::SdlRenderHost(Window& window)
    : _windowOwner(window)
{
    const Size size = window.getSize();
    if (!SDL_CreateWindowAndRenderer(
            window.getTitle().c_str(),
            size.width,
            size.height,
            window.getFlags(),
            &_window,
            &_sdlRenderer)) {
        SDL_Log("Failed to create window and renderer: %s", SDL_GetError());
        return;
    }

    SDL_SetWindowBordered(_window, true);
    window.attachNativeWindow(_window);

    _renderer.reset(gfx::Renderer::create());
    createRenderTarget(size);
}

SdlRenderHost::~SdlRenderHost()
{
    if (_renderer != nullptr) {
        if (_surface != nullptr) {
            _renderer->releaseRenderTarget();
        }
        _renderer.reset();
    }

    destroyTexture();

    if (_sdlRenderer != nullptr) {
        SDL_DestroyRenderer(_sdlRenderer);
        _sdlRenderer = nullptr;
    }

    if (_window != nullptr) {
        _windowOwner.detachNativeWindow(_window);
        SDL_DestroyWindow(_window);
        _window = nullptr;
    }
}

Size SdlRenderHost::getSize() const {
    return _renderTargetSize;
}

bool SdlRenderHost::isReady() const
{
    return _window != nullptr
        && _sdlRenderer != nullptr
        && _texture != nullptr
        && _surface != nullptr
        && _renderer != nullptr;
}

void SdlRenderHost::resize(Size size)
{
    if (!size.hasSize()) {
        return;
    }

    if (size.width == _renderTargetSize.width && size.height == _renderTargetSize.height) {
        return;
    }

    createRenderTarget(size);
}

void SdlRenderHost::renderFrame(Window& window)
{
    if (!isReady()) {
        return;
    }

    window.render(_renderer.get(), {0, 0});

    if (!SDL_UpdateTexture(_texture, nullptr, _surface->getData(), _surface->getPitch())) {
        SDL_Log("Failed to update texture: %s", SDL_GetError());
        return;
    }

    SDL_RenderClear(_sdlRenderer);
    SDL_RenderTexture(_sdlRenderer, _texture, nullptr, nullptr);
    SDL_RenderPresent(_sdlRenderer);
}

void SdlRenderHost::createRenderTarget(Size size)
{
    if (!size.hasSize() || _sdlRenderer == nullptr || _renderer == nullptr) {
        return;
    }

    gfx::ImageInfo info{.dimension = {size.width, size.height}};
    auto surface = gfx::Surface::create(info);
    if (surface == nullptr) {
        SDL_Log("Failed to create render surface.");
        return;
    }

    if (_surface != nullptr) {
        _renderer->releaseRenderTarget();
    }
    _surface = std::move(surface);
    _renderer->bindRenderTarget(_surface);

    destroyTexture();
    _texture = SDL_CreateTexture(
        _sdlRenderer,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        size.width,
        size.height
    );
    if (_texture == nullptr) {
        SDL_Log("Failed to create texture: %s", SDL_GetError());
        return;
    }

    _renderTargetSize = size;
}

void SdlRenderHost::destroyTexture()
{
    if (_texture != nullptr) {
        SDL_DestroyTexture(_texture);
        _texture = nullptr;
    }
}

} // namespace ui
