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
    void setColor(BLRgba32 color) {
        _color = color;
    }

    void render(BLContext& context, Position offset) override;
protected:
    BLRgba32 _color = Colors::WHITE;
};

} // ui
