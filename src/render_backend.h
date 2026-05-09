#pragma once

#include <cstdint>
#include <memory>

#include "layout.h"
#include "render_types.h"

namespace ui {

class RenderStrategy;
class Widget;

class RenderBackend {
public:
    virtual ~RenderBackend() = default;

    virtual void resize(Size size) = 0;
    virtual void beginFrame(Color clearColor) = 0;
    virtual void renderWidget(Widget& widget) = 0;
    virtual void endFrame() = 0;

    virtual const void* pixels() const = 0;
    virtual int32_t stride() const = 0;
    virtual RenderStrategy& strategy() = 0;
};

std::unique_ptr<RenderBackend> createDefaultRenderBackend(Size size);

} // namespace ui
