//
// Created by daniel on 02/08/2025.
//

#pragma once

#include <cassert>

#include "widget.h"

namespace ui {

class Text : public LeafWidget {
public:
    enum class TextAlignment {
        Left,
        Center,
        Right
    };

    Text(std::string value);

    void setValue(std::string value) { _value = std::move(value); markNeedsUpdate(); }
    void setColor(Color color) { _color = color; }
    void setFontSize(int32_t fontSize) { _fontSize = fontSize; }
    void setFontFilepath(std::string filepath) { _fontFilepath = std::move(filepath); }
    void setHorizontalAlignment(TextAlignment alignment) { _horizontalAlignment = alignment; }
    Size layout(const BoxConstraints& boxConstraints) override;
    void render(Position offset) override;
private:
    std::string _value;
    Color _color = Colors::WHITE;
    Color _backgoundColor = Colors::TRANSPARENT;
    int32_t _fontSize = 24;
    std::string _fontFilepath = "resources/fonts/roboto/static/Roboto-Regular.ttf";
    TextAlignment _horizontalAlignment = TextAlignment::Left;
};

} // ui
