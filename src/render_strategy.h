#pragma once

#include <string>

#include <blend2d.h>

#include "layout.h"
#include "render_types.h"

namespace ui {

class RenderStrategy {
public:
    virtual ~RenderStrategy() = default;

    virtual void drawWindow(BLContext& context, const Size& size, BLRgba32 color) = 0;
    virtual void drawContainer(BLContext& context, Position offset, const Size& size, BLRgba32 color) = 0;
    virtual void drawFlexContainer(BLContext& context, Position offset, const Size& size, BLRgba32 color) = 0;
    virtual void drawCenter(BLContext& context, Position offset, const Size& size, BLRgba32 color) = 0;
    virtual void drawAlign(BLContext& context, Position offset, const Size& size, BLRgba32 color) = 0;
    virtual void drawPadding(BLContext& context, Position offset, const Size& size, BLRgba32 color) = 0;
    virtual void drawGrid(BLContext& context, Position offset, const Size& size, BLRgba32 color) = 0;
    virtual void drawButton(
        BLContext& context,
        Position offset,
        const Size& size,
        BLRgba32 backgroundColor,
        BLRgba32 borderColor
    ) = 0;
    virtual void drawText(
        BLContext& context,
        Position offset,
        const Size& size,
        const std::string& value,
        BLRgba32 textColor,
        BLRgba32 backgroundColor,
        int32_t fontSize,
        const std::string& fontFilepath,
        int32_t horizontalAlignment
    ) = 0;
    virtual void drawConstrainedBox(BLContext& context, Position offset, const Size& size) = 0;
    virtual void drawUnconstrainedBox(BLContext& context, Position offset, const Size& size) = 0;
    virtual void drawLimitedBox(BLContext& context, Position offset, const Size& size) = 0;
    virtual void drawOverlay(BLContext& context, Position offset, const Size& size) = 0;
    virtual void drawCalculator(BLContext& context, Position offset, const Size& size) = 0;
};

class Blend2DRenderStrategy final : public RenderStrategy {
public:
    void drawWindow(BLContext& context, const Size& size, BLRgba32 color) override;
    void drawContainer(BLContext& context, Position offset, const Size& size, BLRgba32 color) override;
    void drawFlexContainer(BLContext& context, Position offset, const Size& size, BLRgba32 color) override;
    void drawCenter(BLContext& context, Position offset, const Size& size, BLRgba32 color) override;
    void drawAlign(BLContext& context, Position offset, const Size& size, BLRgba32 color) override;
    void drawPadding(BLContext& context, Position offset, const Size& size, BLRgba32 color) override;
    void drawGrid(BLContext& context, Position offset, const Size& size, BLRgba32 color) override;
    void drawButton(
        BLContext& context,
        Position offset,
        const Size& size,
        BLRgba32 backgroundColor,
        BLRgba32 borderColor
    ) override;
    void drawText(
        BLContext& context,
        Position offset,
        const Size& size,
        const std::string& value,
        BLRgba32 textColor,
        BLRgba32 backgroundColor,
        int32_t fontSize,
        const std::string& fontFilepath,
        int32_t horizontalAlignment
    ) override;
    void drawConstrainedBox(BLContext& context, Position offset, const Size& size) override;
    void drawUnconstrainedBox(BLContext& context, Position offset, const Size& size) override;
    void drawLimitedBox(BLContext& context, Position offset, const Size& size) override;
    void drawOverlay(BLContext& context, Position offset, const Size& size) override;
    void drawCalculator(BLContext& context, Position offset, const Size& size) override;

private:
    static void drawRectBackground(BLContext& context, Position offset, const Size& size, BLRgba32 color);
};

} // namespace ui
