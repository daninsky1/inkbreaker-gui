//
// Created by daniel on 28/04/2026.
//

#pragma once

#include <functional>

#include "widget.h"

namespace ui {

class Button : public SingleChildWidget {
public:
    using Callback = std::function<void()>;

    Button() = default;
    explicit Button(Widget& child);

    void setOnPressed(Callback callback) { _onPressed = std::move(callback); }
    void setPadding(BoxSpace padding) { _padding = padding; }
    void setColor(BLRgba32 color) { _backgroundColor = color; }
    void setHoverColor(BLRgba32 color) { _hoverColor = color; }
    void setPressedColor(BLRgba32 color) { _pressedColor = color; }

    Size layout(const BoxConstraints& boxConstraints) override;
    void render(BLContext& context, Position offset) override;
    Event& eventHandler(Event& event) override;

private:
    [[nodiscard]] bool contains(Position position) const;
    [[nodiscard]] BLRgba32 currentColor() const;

    Callback _onPressed;
    BoxSpace _padding = {8, 16, 8, 16};
    BLRgba32 _backgroundColor = BLRgba32{37, 99, 235, 255};
    BLRgba32 _hoverColor = BLRgba32{29, 78, 216, 255};
    BLRgba32 _pressedColor = BLRgba32{30, 64, 175, 255};
    BLRgba32 _borderColor = BLRgba32{147, 197, 253, 255};
    bool _hovered = false;
    bool _pressed = false;
};

} // ui
