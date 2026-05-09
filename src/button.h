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
    void setColor(Color color) { _backgroundColor = color; }
    void setHoverColor(Color color) { _hoverColor = color; }
    void setPressedColor(Color color) { _pressedColor = color; }

    Size layout(const BoxConstraints& boxConstraints) override;
    void render(Position offset) override;
    Event& eventHandler(Event& event) override;

private:
    [[nodiscard]] bool contains(Position position) const;
    [[nodiscard]] Color currentColor() const;

    Callback _onPressed;
    BoxSpace _padding = {8, 16, 8, 16};
    Color _backgroundColor = Color{37, 99, 235, 255};
    Color _hoverColor = Color{29, 78, 216, 255};
    Color _pressedColor = Color{30, 64, 175, 255};
    Color _borderColor = Color{147, 197, 253, 255};
    bool _hovered = false;
    bool _pressed = false;
};

} // ui
