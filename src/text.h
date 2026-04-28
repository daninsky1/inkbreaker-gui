//
// Created by daniel on 02/08/2025.
//

#pragma once

#include <cassert>

#include "widget.h"

namespace ui {

class Text : public LeafWidget {
public:
    Text(std::string value);

    void setValue(std::string value) { _value = std::move(value); markNeedsUpdate(); }
    void setColor(BLRgba32 color) { _color = color; }
    void setFontSize(int32_t fontSize) { _fontSize = fontSize; }
    void setFontFilepath(std::string filepath) { _fontFilepath = std::move(filepath); }
    Size layout(const BoxConstraints& boxConstraints) override;
    void render(BLContext& context, Position offset) override;
private:
    std::string _value;
    BLRgba32 _color = Colors::WHITE;
    BLRgba32 _backgoundColor = Colors::TRANSPARENT;
    int32_t _fontSize = 24;
    std::string _fontFilepath = "resources/fonts/roboto/static/Roboto-Regular.ttf";
};

} // ui
