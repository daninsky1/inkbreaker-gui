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
