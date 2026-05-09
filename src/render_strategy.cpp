#include "render_strategy.h"

namespace ui {

void Blend2DRenderStrategy::drawWindow(BLContext& context, const Size& size, BLRgba32 color)
{
    (void)size;
    context.fillAll(color);
}

void Blend2DRenderStrategy::drawContainer(BLContext& context, Position offset, const Size& size, BLRgba32 color)
{
    drawRectBackground(context, offset, size, color);
}

void Blend2DRenderStrategy::drawFlexContainer(BLContext& context, Position offset, const Size& size, BLRgba32 color)
{
    drawRectBackground(context, offset, size, color);
}

void Blend2DRenderStrategy::drawCenter(BLContext& context, Position offset, const Size& size, BLRgba32 color)
{
    drawRectBackground(context, offset, size, color);
}

void Blend2DRenderStrategy::drawAlign(BLContext& context, Position offset, const Size& size, BLRgba32 color)
{
    drawRectBackground(context, offset, size, color);
}

void Blend2DRenderStrategy::drawPadding(BLContext& context, Position offset, const Size& size, BLRgba32 color)
{
    drawRectBackground(context, offset, size, color);
}

void Blend2DRenderStrategy::drawGrid(BLContext& context, Position offset, const Size& size, BLRgba32 color)
{
    drawRectBackground(context, offset, size, color);
}

void Blend2DRenderStrategy::drawButton(
    BLContext& context,
    Position offset,
    const Size& size,
    BLRgba32 backgroundColor,
    BLRgba32 borderColor
)
{
    if (size.width <= 0 || size.height <= 0) {
        return;
    }

    context.save();
    context.translate(offset.x, offset.y);
    context.clipToRect(BLRectI{0, 0, size.width, size.height});
    context.fillRoundRect(
        BLRoundRect{0.0, 0.0, static_cast<double>(size.width), static_cast<double>(size.height), 4.0},
        backgroundColor
    );
    if (size.width > 1 && size.height > 1) {
        context.strokeRoundRect(
            BLRoundRect{0.5, 0.5, static_cast<double>(size.width) - 1.0, static_cast<double>(size.height) - 1.0, 4.0},
            borderColor
        );
    }
    context.restore();
}

void Blend2DRenderStrategy::drawText(
    BLContext& context,
    Position offset,
    const Size& size,
    const std::string& value,
    BLRgba32 textColor,
    BLRgba32 backgroundColor,
    int32_t fontSize,
    const std::string& fontFilepath,
    int32_t horizontalAlignment
)
{
    context.save();
    context.translate(offset.x, offset.y);
    context.clipToRect(BLRectI{0, 0, size.width, size.height});
    context.fillRect(BLRectI{0, 0, size.width, size.height}, backgroundColor);

    BLFontFace fontFace;
    if (fontFace.createFromFile(fontFilepath.c_str()) != BL_SUCCESS) {
        context.restore();
        return;
    }

    BLFont font;
    font.createFromFace(fontFace, static_cast<float>(fontSize));

    const BLFontMetrics fontMetrics = font.metrics();
    const double lineHeight = fontMetrics.ascent + fontMetrics.descent + fontMetrics.lineGap;
    double y = fontMetrics.ascent;
    BLGlyphBuffer glyphBuffer;
    BLTextMetrics textMetrics;

    size_t lineStart = 0;
    while (lineStart <= value.size()) {
        const size_t lineEnd = value.find('\n', lineStart);
        const size_t lineSize = lineEnd == std::string::npos
            ? value.size() - lineStart
            : lineEnd - lineStart;

        if (lineSize > 0) {
            glyphBuffer.setUtf8Text(value.data() + lineStart, lineSize);
            font.shape(glyphBuffer);
            font.getTextMetrics(glyphBuffer, textMetrics);

            double x = 0.0;
            if (horizontalAlignment == 1) {
                x = (static_cast<double>(size.width) - textMetrics.advance.x) / 2.0;
            } else if (horizontalAlignment == 2) {
                x = static_cast<double>(size.width) - textMetrics.advance.x;
            }

            context.fillUtf8Text(
                BLPoint{x, y},
                font,
                value.data() + lineStart,
                lineSize,
                textColor
            );
        }

        if (lineEnd == std::string::npos) {
            break;
        }
        lineStart = lineEnd + 1;
        y += lineHeight;
    }

    context.restore();
}

void Blend2DRenderStrategy::drawConstrainedBox(BLContext& context, Position offset, const Size& size)
{
    (void)context;
    (void)offset;
    (void)size;
}

void Blend2DRenderStrategy::drawUnconstrainedBox(BLContext& context, Position offset, const Size& size)
{
    (void)context;
    (void)offset;
    (void)size;
}

void Blend2DRenderStrategy::drawLimitedBox(BLContext& context, Position offset, const Size& size)
{
    (void)context;
    (void)offset;
    (void)size;
}

void Blend2DRenderStrategy::drawOverlay(BLContext& context, Position offset, const Size& size)
{
    (void)context;
    (void)offset;
    (void)size;
}

void Blend2DRenderStrategy::drawCalculator(BLContext& context, Position offset, const Size& size)
{
    (void)context;
    (void)offset;
    (void)size;
}

void Blend2DRenderStrategy::drawRectBackground(BLContext& context, Position offset, const Size& size, BLRgba32 color)
{
    if (size.width <= 0 || size.height <= 0) {
        return;
    }

    context.save();
    context.translate(offset.x, offset.y);
    context.clipToRect(BLRectI{0, 0, size.width, size.height});
    context.fillRect(BLRectI{0, 0, size.width, size.height}, color);
    context.restore();
}

} // namespace ui
