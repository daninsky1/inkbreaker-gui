#pragma once

#include <blend2d.h>

#include "render_backend.h"
#include "render_strategy.h"

namespace ui {

class Blend2DRenderStrategy final : public RenderStrategy {
public:
    void setContext(BLContext* context) { _context = context; }

    Size measureText(const std::string& value, int32_t fontSize, const std::string& fontFilepath) override;

    void drawWindow(const Size& size, Color color) override;
    void drawContainer(Position offset, const Size& size, Color color) override;
    void drawFlexContainer(Position offset, const Size& size, Color color) override;
    void drawCenter(Position offset, const Size& size, Color color) override;
    void drawAlign(Position offset, const Size& size, Color color) override;
    void drawPadding(Position offset, const Size& size, Color color) override;
    void drawGrid(Position offset, const Size& size, Color color) override;
    void drawButton(Position offset, const Size& size, Color backgroundColor, Color borderColor) override;
    void drawText(
        Position offset,
        const Size& size,
        const std::string& value,
        Color textColor,
        Color backgroundColor,
        int32_t fontSize,
        const std::string& fontFilepath,
        int32_t horizontalAlignment
    ) override;
    void drawConstrainedBox(Position offset, const Size& size) override;
    void drawUnconstrainedBox(Position offset, const Size& size) override;
    void drawLimitedBox(Position offset, const Size& size) override;
    void drawOverlay(Position offset, const Size& size) override;
    void drawCalculator(Position offset, const Size& size) override;

private:
    static BLRgba32 toBlendColor(Color color);
    void drawRectBackground(Position offset, const Size& size, Color color);

    BLContext* _context = nullptr;
};

class Blend2DRenderBackend final : public RenderBackend {
public:
    explicit Blend2DRenderBackend(Size size);

    void resize(Size size) override;
    void beginFrame(Color clearColor) override;
    void renderWidget(Widget& widget) override;
    void endFrame() override;

    const void* pixels() const override;
    int32_t stride() const override;
    RenderStrategy& strategy() override { return _strategy; }

private:
    Size _size{};
    BLImage _image;
    BLImageData _imageData{};
    BLContext _context;
    Blend2DRenderStrategy _strategy;
};

} // namespace ui
