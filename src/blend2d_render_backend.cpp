#include "blend2d_render_backend.h"

#include <algorithm>
#include <cmath>

#include "widget.h"

namespace ui {

namespace {
int32_t ceilToInt(double value)
{
    return static_cast<int32_t>(std::ceil(value));
}
} // namespace

Size Blend2DRenderStrategy::measureText(const std::string& value, int32_t fontSize, const std::string& fontFilepath)
{
    BLFontFace fontFace;
    if (fontFace.createFromFile(fontFilepath.c_str()) != BL_SUCCESS) {
        return {};
    }

    BLFont font;
    font.createFromFace(fontFace, static_cast<float>(fontSize));

    const BLFontMetrics fontMetrics = font.metrics();
    const double lineHeight = fontMetrics.ascent + fontMetrics.descent + fontMetrics.lineGap;

    int32_t width = 0;
    int32_t lineCount = 0;
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
            width = std::max(width, ceilToInt(textMetrics.advance.x));
        }

        ++lineCount;
        if (lineEnd == std::string::npos) {
            break;
        }
        lineStart = lineEnd + 1;
    }

    return {
        .width = width,
        .height = ceilToInt(lineHeight * lineCount)
    };
}

void Blend2DRenderStrategy::drawWindow(const Size& size, Color color)
{
    (void)size;
    if (_context != nullptr) {
        _context->fillAll(toBlendColor(color));
    }
}

void Blend2DRenderStrategy::drawContainer(Position offset, const Size& size, Color color)
{
    drawRectBackground(offset, size, color);
}

void Blend2DRenderStrategy::drawFlexContainer(Position offset, const Size& size, Color color)
{
    drawRectBackground(offset, size, color);
}

void Blend2DRenderStrategy::drawCenter(Position offset, const Size& size, Color color)
{
    drawRectBackground(offset, size, color);
}

void Blend2DRenderStrategy::drawAlign(Position offset, const Size& size, Color color)
{
    drawRectBackground(offset, size, color);
}

void Blend2DRenderStrategy::drawPadding(Position offset, const Size& size, Color color)
{
    drawRectBackground(offset, size, color);
}

void Blend2DRenderStrategy::drawGrid(Position offset, const Size& size, Color color)
{
    drawRectBackground(offset, size, color);
}

void Blend2DRenderStrategy::drawButton(Position offset, const Size& size, Color backgroundColor, Color borderColor)
{
    if (_context == nullptr || size.width <= 0 || size.height <= 0) {
        return;
    }

    _context->save();
    _context->translate(offset.x, offset.y);
    _context->clipToRect(BLRectI{0, 0, size.width, size.height});
    _context->fillRoundRect(
        BLRoundRect{0.0, 0.0, static_cast<double>(size.width), static_cast<double>(size.height), 4.0},
        toBlendColor(backgroundColor)
    );
    if (size.width > 1 && size.height > 1) {
        _context->strokeRoundRect(
            BLRoundRect{0.5, 0.5, static_cast<double>(size.width) - 1.0, static_cast<double>(size.height) - 1.0, 4.0},
            toBlendColor(borderColor)
        );
    }
    _context->restore();
}

void Blend2DRenderStrategy::drawText(
    Position offset,
    const Size& size,
    const std::string& value,
    Color textColor,
    Color backgroundColor,
    int32_t fontSize,
    const std::string& fontFilepath,
    int32_t horizontalAlignment
)
{
    if (_context == nullptr) {
        return;
    }

    _context->save();
    _context->translate(offset.x, offset.y);
    _context->clipToRect(BLRectI{0, 0, size.width, size.height});
    _context->fillRect(BLRectI{0, 0, size.width, size.height}, toBlendColor(backgroundColor));

    BLFontFace fontFace;
    if (fontFace.createFromFile(fontFilepath.c_str()) != BL_SUCCESS) {
        _context->restore();
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

            _context->fillUtf8Text(
                BLPoint{x, y},
                font,
                value.data() + lineStart,
                lineSize,
                toBlendColor(textColor)
            );
        }

        if (lineEnd == std::string::npos) {
            break;
        }
        lineStart = lineEnd + 1;
        y += lineHeight;
    }

    _context->restore();
}

void Blend2DRenderStrategy::drawConstrainedBox(Position offset, const Size& size) { (void)offset; (void)size; }
void Blend2DRenderStrategy::drawUnconstrainedBox(Position offset, const Size& size) { (void)offset; (void)size; }
void Blend2DRenderStrategy::drawLimitedBox(Position offset, const Size& size) { (void)offset; (void)size; }
void Blend2DRenderStrategy::drawOverlay(Position offset, const Size& size) { (void)offset; (void)size; }
void Blend2DRenderStrategy::drawCalculator(Position offset, const Size& size) { (void)offset; (void)size; }

BLRgba32 Blend2DRenderStrategy::toBlendColor(Color color)
{
    return BLRgba32{color.r, color.g, color.b, color.a};
}

void Blend2DRenderStrategy::drawRectBackground(Position offset, const Size& size, Color color)
{
    if (_context == nullptr || size.width <= 0 || size.height <= 0) {
        return;
    }

    _context->save();
    _context->translate(offset.x, offset.y);
    _context->clipToRect(BLRectI{0, 0, size.width, size.height});
    _context->fillRect(BLRectI{0, 0, size.width, size.height}, toBlendColor(color));
    _context->restore();
}

Blend2DRenderBackend::Blend2DRenderBackend(Size size)
{
    resize(size);
}

void Blend2DRenderBackend::resize(Size size)
{
    _size = size;
    _image = BLImage(size.width, size.height, BL_FORMAT_PRGB32);
    _image.getData(&_imageData);
}

void Blend2DRenderBackend::beginFrame(Color clearColor)
{
    _context = BLContext(_image);
    _strategy.setContext(&_context);
    _strategy.drawWindow(_size, clearColor);
}

void Blend2DRenderBackend::renderWidget(Widget& widget)
{
    widget.setRenderStrategy(_strategy);
    widget.render({0, 0});
}

void Blend2DRenderBackend::endFrame()
{
    _context.end();
    _strategy.setContext(nullptr);
    _image.getData(&_imageData);
}

const void* Blend2DRenderBackend::pixels() const
{
    return _imageData.pixelData;
}

int32_t Blend2DRenderBackend::stride() const
{
    return static_cast<int32_t>(_imageData.stride);
}

std::unique_ptr<RenderBackend> createDefaultRenderBackend(Size size)
{
    return std::make_unique<Blend2DRenderBackend>(size);
}

} // namespace ui
