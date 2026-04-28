//
// Created by daniel on 28/04/2026.
//

#include "button.h"

#include <algorithm>

#include <SDL3/SDL_mouse.h>

namespace ui {

Button::Button(Widget& child)
{
    setChild(child);
}

Size Button::layout(const BoxConstraints& boxConstraints)
{
    const int32_t horizontalPadding = _padding.left + _padding.right;
    const int32_t verticalPadding = _padding.top + _padding.bottom;

    BoxConstraints childConstraints = {
        .minWidth = 0,
        .minHeight = 0,
        .maxWidth = std::max(0, boxConstraints.maxWidth - horizontalPadding),
        .maxHeight = std::max(0, boxConstraints.maxHeight - verticalPadding)
    };

    Size childSize = {0, 0};
    if (_child != nullptr) {
        childSize = _child->layout(childConstraints);
    }

    _childPosition = {
        .x = _padding.left,
        .y = _padding.top
    };

    if (_child == nullptr) {
        _size = {96, 40};
    } else {
        _size = {
            .width = childSize.width + horizontalPadding,
            .height = childSize.height + verticalPadding
        };
    }

    return normalize(boxConstraints);
}

void Button::render(BLContext& context, Position offset)
{
    if (_size.width <= 0 || _size.height <= 0) {
        return;
    }

    context.save();
    context.translate(offset.x, offset.y);
    context.clipToRect(BLRectI{0, 0, _size.width, _size.height});
    context.fillRoundRect(BLRoundRect{0.0, 0.0, static_cast<double>(_size.width), static_cast<double>(_size.height), 4.0}, currentColor());
    if (_size.width > 1 && _size.height > 1) {
        context.strokeRoundRect(BLRoundRect{0.5, 0.5, static_cast<double>(_size.width) - 1.0, static_cast<double>(_size.height) - 1.0, 4.0}, _borderColor);
    }
    context.restore();

    if (_child != nullptr) {
        _child->render(context, offset.add(_childPosition));
    }
}

Event& Button::eventHandler(Event& event)
{
    if (!event.hasPosition) {
        return event;
    }

    const bool inside = contains(event.position);

    switch (event.sdlEvent.type) {
    case SDL_EVENT_MOUSE_MOTION:
        _hovered = inside;
        break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        if (inside && event.sdlEvent.button.button == SDL_BUTTON_LEFT) {
            _pressed = true;
            event.handled = true;
        }
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        if (_pressed && event.sdlEvent.button.button == SDL_BUTTON_LEFT) {
            _pressed = false;
            if (inside && _onPressed) {
                _onPressed();
            }
            event.handled = true;
        }
        _hovered = inside;
        break;
    default:
        break;
    }

    return event;
}

bool Button::contains(Position position) const
{
    return position.x >= 0 &&
           position.y >= 0 &&
           position.x < _size.width &&
           position.y < _size.height;
}

BLRgba32 Button::currentColor() const
{
    if (_pressed) {
        return _pressedColor;
    }
    if (_hovered) {
        return _hoverColor;
    }
    return _backgroundColor;
}

} // ui
