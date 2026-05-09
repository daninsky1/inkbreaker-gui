//
// Created by daniel on 02/08/2025.
//

#include "text.h"

#include "render_strategy.h"

namespace ui {

Text::Text(std::string value)
    : _value(std::move(value))
{

}

Size Text::layout(const BoxConstraints& boxConstraints)
{
    _size = getRenderStrategy().measureText(_value, _fontSize, _fontFilepath);
    return normalize(boxConstraints);
}

void Text::render(Position offset)
{
    getRenderStrategy().drawText(
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
