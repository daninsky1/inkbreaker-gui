//
// Created by daniel on 27/07/2025.
//

#include "align.h"

#include "render_strategy.h"

namespace ui
{
Align::Align()
{

}


Size Align::layout(const BoxConstraints& constraint)
{
    BoxConstraints childConstraint = {
        .minWidth = 0,
        .minHeight = 0,
        .maxWidth = constraint.maxWidth,
        .maxHeight = constraint.maxHeight
    };
    if (_child != nullptr) {
        Size childSize = _child->layout(childConstraint);

        int32_t x = 0;
        int32_t y = 0;

        switch (_alignment) {
        case Alignment::TopLeft:
            x = 0;
            y = 0;
            break;
        case Alignment::TopCenter:
            x = (constraint.maxWidth - childSize.width) / 2;
            y = 0;
            break;
        case Alignment::TopRight:
            x = constraint.maxWidth - childSize.width;
            y = 0;
            break;
        case Alignment::CenterLeft:
            x = 0;
            y = (constraint.maxHeight - childSize.height) / 2;
            break;
        case Alignment::Center:
            x = (constraint.maxWidth - childSize.width) / 2;
            y = (constraint.maxHeight - childSize.height) / 2;
            break;
        case Alignment::CenterRight:
            x = constraint.maxWidth - childSize.width;
            y = (constraint.maxHeight - childSize.height) / 2;
            break;
        case Alignment::BottomLeft:
            x = 0;
            y = constraint.maxHeight - childSize.height;
            break;
        case Alignment::BottomCenter:
            x = (constraint.maxWidth - childSize.width) / 2;
            y = constraint.maxHeight - childSize.height;
            break;
        case Alignment::BottomRight:
            x = constraint.maxWidth - childSize.width;
            y = constraint.maxHeight - childSize.height;
            break;
        }

        _childPosition = { .x = x, .y = y };
    }
    // Normalize Align size
    return normalize(constraint);
}

void Align::render(Position offset)
{
    getRenderStrategy().drawAlign(offset, _size, _color);

    if (_child != nullptr) {
        _child->render(_childPosition.add(offset));
    }
}


}   // namespace ui
