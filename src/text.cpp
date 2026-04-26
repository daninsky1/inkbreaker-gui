//
// Created by daniel on 02/08/2025.
//

#include "text.h"

namespace ui {

Text::Text(std::string value)
    : _value(std::move(value))
{

}

Size Text::layout(const BoxConstraints& boxConstraints)
{
    return _size;
}

void Text::render(BLContext& context, Position offset)
{
    context.save();
    context.translate(offset.x, offset.y);
    context.clipToRect(BLRectI{0, 0, _size.width, _size.height});
    context.fillRect(BLRectI{0, 0, _size.width, _size.height}, _backgoundColor);

    BLFontFace fontFace;
    fontFace.createFromFile(_fontFilepath.c_str());
    BLFont font;
    font.createFromFace(fontFace, static_cast<float>(_fontSize));

    

    // TODO(Daniel S): Renderizar texto


    context.restore();
}

} // ui
