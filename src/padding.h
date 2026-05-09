//
// Created by daniel on 27/07/2025.
//

#pragma once

#include "layout.h"
#include "widget.h"


namespace ui {

class Padding : public SingleChildWidget {
public:
    Padding();
    Padding(uint32_t top, uint32_t left, uint32_t bottom, uint32_t right);
    Size layout(const BoxConstraints& constraint) override;
    void render(Position offset) override;

    void setColor(Color color) { _color = color; }
    void setPadding(BoxSpace padding) { _padding = padding; }
    BoxSpace getPadding() const { return _padding; }
protected:
    BoxSpace _padding = {0, 0, 0, 0};
    Color _color = Colors::WHITE;
};
} // ui
