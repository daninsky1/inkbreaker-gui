//
// Created by daniel on 02/08/2025.
//

#include "text.h"

#include <cmath>

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
    const double lineHeight = fontMetrics.ascent + fontMetrics.descent + fontMetrics.lineGap;

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

void Text::render(BLContext& context, Position offset)
{
    context.save();
    context.translate(offset.x, offset.y);
    context.clipToRect(BLRectI{0, 0, _size.width, _size.height});
    context.fillRect(BLRectI{0, 0, _size.width, _size.height}, _backgoundColor);

    BLFontFace fontFace;
    if (fontFace.createFromFile(_fontFilepath.c_str()) != BL_SUCCESS) {
        context.restore();
        return;
    }

    BLFont font;
    font.createFromFace(fontFace, static_cast<float>(_fontSize));

    const BLFontMetrics fontMetrics = font.metrics();
    const double lineHeight = fontMetrics.ascent + fontMetrics.descent + fontMetrics.lineGap;
    double y = fontMetrics.ascent;

    size_t lineStart = 0;
    while (lineStart <= _value.size()) {
        const size_t lineEnd = _value.find('\n', lineStart);
        const size_t lineSize = lineEnd == std::string::npos
            ? _value.size() - lineStart
            : lineEnd - lineStart;

        if (lineSize > 0) {
            context.fillUtf8Text(
                BLPoint{0.0, y},
                font,
                _value.data() + lineStart,
                lineSize,
                _color
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

} // ui
