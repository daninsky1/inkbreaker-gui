#pragma once

#include <vector>

#include "widget.h"

namespace ui
{

class Overlay : public MultiChildWidget
{
public:
    Overlay() = default;
    explicit Overlay(const std::vector<Widget*>& children);

    Size layout(const BoxConstraints& boxConstraints) override;
    void render(gfx::Renderer* renderer, Position offset) override;
    Event& eventHandler(Event& event) override;
};

} // namespace ui
