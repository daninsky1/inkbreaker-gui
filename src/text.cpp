//
// Created by daniel on 02/08/2025.
//

#include "text.h"

#include "renderer.h"

namespace ui {

Text::Text(std::string value)
    : _value(std::move(value))
{

}

Size Text::layout(const BoxConstraints& boxConstraints)
{
    _size = getRenderer().measureText(_value, _fontSize, _fontFilepath);
    return normalize(boxConstraints);
}

void Text::render(Position offset)
{
    getRenderer().drawText(
        offset,
        _size,
        _value,
        _color,
        _backgoundColor,
        _fontSize,
        _fontFilepath,
        static_cast<int32_t>(_horizontalAlignment)
    );
}

} // ui
