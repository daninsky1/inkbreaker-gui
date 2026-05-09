//
// Created by daniel on 28/04/2026.
//

#include "button.h"

#include <algorithm>

#include <SDL3/SDL_mouse.h>

#include "render_strategy.h"

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

    const Size normalizedSize = normalize(boxConstraints);

    if (_child != nullptr) {
        const int32_t contentWidth = std::max(0, _size.width - horizontalPadding);
        const int32_t contentHeight = std::max(0, _size.height - verticalPadding);
        _childPosition = {
            .x = _padding.left + std::max(0, (contentWidth - childSize.width) / 2),
            .y = _padding.top + std::max(0, (contentHeight - childSize.height) / 2)
        };
    }

    return normalizedSize;
}

void Button::render(Position offset)
{
    getRenderStrategy().drawButton(offset, _size, currentColor(), _borderColor);

    if (_child != nullptr) {
        _child->render(offset.add(_childPosition));
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

Color Button::currentColor() const
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
