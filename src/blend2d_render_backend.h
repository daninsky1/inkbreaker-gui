#pragma once

#include <blend2d.h>

#include "render_backend.h"
#include "renderer.h"

namespace ui {

class Blend2DRenderer final {
public:
    void setContext(BLContext* context) { _context = context; }

    Size measureText(const std::string& value, int32_t fontSize, const std::string& fontFilepath);

    void drawWindow(const Size& size, Color color);
    void drawContainer(Position offset, const Size& size, Color color);
    void drawFlexContainer(Position offset, const Size& size, Color color);
    void drawCenter(Position offset, const Size& size, Color color);
    void drawAlign(Position offset, const Size& size, Color color);
    void drawPadding(Position offset, const Size& size, Color color);
    void drawGrid(Position offset, const Size& size, Color color);
    void drawButton(Position offset, const Size& size, Color backgroundColor, Color borderColor);
    void drawText(
        Position offset,
        const Size& size,
        const std::string& value,
        Color textColor,
        Color backgroundColor,
        int32_t fontSize,
        const std::string& fontFilepath,
        int32_t horizontalAlignment
    );
    void drawConstrainedBox(Position offset, const Size& size);
    void drawUnconstrainedBox(Position offset, const Size& size);
    void drawLimitedBox(Position offset, const Size& size);
    void drawOverlay(Position offset, const Size& size);
    void drawCalculator(Position offset, const Size& size);

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
    Renderer& renderer() override { return _renderer; }

private:
    Size _size{};
    BLImage _image;
    BLImageData _imageData{};
    BLContext _context;
    Blend2DRenderer _rendererImpl;
    Renderer _renderer;
};

} // namespace ui
