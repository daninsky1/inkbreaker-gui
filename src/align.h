//
// Created by daniel on 27/07/2025.
//
#pragma once

#include "widget.h"

namespace ui
{

enum class Alignment
{
    TopLeft,
    TopCenter,
    TopRight,
    CenterLeft,
    Center,
    CenterRight,
    BottomLeft,
    BottomCenter,
    BottomRight
};

class Align : public SingleChildWidget
{
public:
    Align();
    Size layout(const BoxConstraints& constraint) override;
    void render(Position offset) override;

    void setColor(Color color) { _color = color; }
    void setAlignment(const Alignment alignment) { _alignment = alignment; }
protected:
    Color _color = Colors::WHITE;
    Alignment _alignment = Alignment::Center;
};

}   // namespace ui
