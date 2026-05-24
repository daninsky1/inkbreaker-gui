//
// Created by daniel on 02/08/2025.
//

#include "text.h"

#include <cmath>
#include <string_view>
#include <blend2d.h>

namespace ui {

namespace {

int32_t ceilToInt(double value)
{
    return static_cast<int32_t>(std::ceil(value));
}

} // namespace

Text::Text(std::string value)
    : _value(std::move(value))
{

}

Size Text::layout(const BoxConstraints& boxConstraints)
{
    BLFontFace fontFace;
    if (fontFace.createFromFile(_fontFilepath.c_str()) != BL_SUCCESS) {
        return normalize(boxConstraints);
    }

    BLFont font;
    font.createFromFace(fontFace, static_cast<float>(_fontSize));

    const BLFontMetrics fontMetrics = font.metrics();
    const double lineHeight =
        fontMetrics.ascent + fontMetrics.descent + fontMetrics.lineGap;

    int32_t width = 0;
    int32_t lineCount = 0;
    BLGlyphBuffer glyphBuffer;
    BLTextMetrics textMetrics;

    size_t lineStart = 0;
    while (lineStart <= _value.size()) {
        const size_t lineEnd = _value.find('\n', lineStart);
        const size_t lineSize = lineEnd == std::string::npos
            ? _value.size() - lineStart
            : lineEnd - lineStart;

        if (lineSize > 0) {
            glyphBuffer.setUtf8Text(_value.data() + lineStart, lineSize);
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

    _size = {
        .width = width,
        .height = ceilToInt(lineHeight * lineCount)
    };

    return normalize(boxConstraints);
}

void Text::render(gfx::Renderer* renderer, Position offset)
{
    renderer->save();
    renderer->translate(offset.x, offset.y);
    renderer->clipRect(gfx::Rect{0, 0, _size.width, _size.height});
    renderer->drawRect(
        gfx::Rect{0, 0, _size.width, _size.height},
        gfx::Paint::create().setStyle(gfx::Style::FILL_STYLE).setColor(_backgoundColor)
    );

    BLFontFace fontFace;
    if (fontFace.createFromFile(_fontFilepath.c_str()) != BL_SUCCESS) {
        renderer->restore();
        return;
    }

    BLFont metricsFont;
    metricsFont.createFromFace(fontFace, static_cast<float>(_fontSize));

    auto typeface = gfx::Typeface::createFromFile(_fontFilepath);
    auto font = gfx::Font::createFromTypeface(typeface, static_cast<float>(_fontSize));
    auto paint = gfx::Paint::create().setStyle(gfx::Style::FILL_STYLE).setColor(_color);

    const BLFontMetrics fontMetrics = metricsFont.metrics();
    const double lineHeight = fontMetrics.ascent + fontMetrics.descent + fontMetrics.lineGap;
    double y = fontMetrics.ascent;
    BLGlyphBuffer glyphBuffer;
    BLTextMetrics textMetrics;

    size_t lineStart = 0;
    while (lineStart <= _value.size()) {
        const size_t lineEnd = _value.find('\n', lineStart);
        const size_t lineSize = lineEnd == std::string::npos
            ? _value.size() - lineStart
            : lineEnd - lineStart;

        if (lineSize > 0) {
            glyphBuffer.setUtf8Text(_value.data() + lineStart, lineSize);
            metricsFont.shape(glyphBuffer);
            metricsFont.getTextMetrics(glyphBuffer, textMetrics);

            double x = 0.0;
            if (_horizontalAlignment == TextAlignment::Center) {
                x = (static_cast<double>(_size.width) - textMetrics.advance.x) / 2.0;
            } else if (_horizontalAlignment == TextAlignment::Right) {
                x = static_cast<double>(_size.width) - textMetrics.advance.x;
            }

            renderer->drawText(
                std::string_view{_value.data() + lineStart, lineSize},
                gfx::Point{ceilToInt(x), ceilToInt(y)},
                *font,
                paint
            );
        }

        if (lineEnd == std::string::npos) {
            break;
        }
        lineStart = lineEnd + 1;
        y += lineHeight;
    }

    renderer->restore();
}

} // ui
