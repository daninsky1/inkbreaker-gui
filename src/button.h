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
    void setColor(gfx::Color color) { _backgroundColor = color; }
    void setHoverColor(gfx::Color color) { _hoverColor = color; }
    void setPressedColor(gfx::Color color) { _pressedColor = color; }

    Size layout(const BoxConstraints& boxConstraints) override;
    void render(gfx::Renderer* renderer, Position offset) override;
    Event& eventHandler(Event& event) override;

private:
    [[nodiscard]] bool contains(Position position) const;
    [[nodiscard]] gfx::Color currentColor() const;

    Callback _onPressed;
    BoxSpace _padding = {8, 16, 8, 16};
    gfx::Color _backgroundColor = gfx::Color{37, 99, 235, 255};
    gfx::Color _hoverColor = gfx::Color{29, 78, 216, 255};
    gfx::Color _pressedColor = gfx::Color{30, 64, 175, 255};
    bool _hovered = false;
    bool _pressed = false;
};

} // ui
