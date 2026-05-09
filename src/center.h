//
// Created by daniel on 26/07/2025.
//
#pragma once

#include "widget.h"

#include "layout.h"

namespace ui {

class Center : public SingleChildWidget {
public:
    Center();
    Size layout(const BoxConstraints& constraint) override;
    void setColor(Color color) {
        _color = color;
    }

    void render(Position offset) override;
protected:
    Color _color = Colors::WHITE;
};

} // ui
