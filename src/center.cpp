//
// Created by daniel on 26/07/2025.
//

#include "center.h"

#include "render_strategy.h"

namespace ui {

Center::Center()
{

}

Size Center::layout(const BoxConstraints& constraint)
{
    BoxConstraints centerConstraint = {
        .minWidth = 0,
        .minHeight = 0,
        .maxWidth = constraint.maxWidth,
        .maxHeight = constraint.maxHeight
    };
    if (_child != nullptr) {
        Size childSize = _child->layout(centerConstraint);
        _childPosition = {
            .x = (constraint.maxWidth - childSize.width) / 2,
            .y = (constraint.maxHeight - childSize.height) / 2
        };
    }
    // Normalize Center size
    return normalize(constraint);
}

void Center::render(Position offset)
{
    getRenderStrategy().drawCenter(offset, _size, _color);

    if (_child != nullptr) {
        _child->render(_childPosition.add(offset));
    }
}

} // ui
