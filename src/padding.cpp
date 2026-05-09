//
// Created by daniel on 27/07/2025.
//

#include "padding.h"

#include "render_strategy.h"

namespace ui {

Padding::Padding()
{

}

Padding::Padding(uint32_t top, uint32_t left, uint32_t bottom, uint32_t right)
    :_padding(top, left, bottom, right)
{
}

Size Padding::layout(const BoxConstraints& constraint)
{
    const int32_t horizontalPadding = _padding.left + _padding.right;
    const int32_t verticalPadding = _padding.top + _padding.bottom;

    BoxConstraints childConstraint = {
        .minWidth = std::max(0, constraint.minWidth > horizontalPadding
                             ? constraint.minWidth - horizontalPadding
                             : 0),
        .minHeight = std::max(0, constraint.minHeight > verticalPadding
                              ? constraint.minHeight - verticalPadding
                              : 0),
        .maxWidth = constraint.maxWidth > horizontalPadding
                    ? constraint.maxWidth - horizontalPadding
                    : 0,
        .maxHeight = constraint.maxHeight > verticalPadding
                     ? constraint.maxHeight - verticalPadding
                     : 0
    };

    Size childSize = { 0, 0 };
    if (_child != nullptr) {
        childSize = _child->layout(childConstraint);

        _childPosition = { .x = _padding.left, .y = _padding.top };
    }

    _size = {
        .width = childSize.width + horizontalPadding,
        .height = childSize.height + verticalPadding
    };
    return normalize(constraint);
}

void Padding::render(BLContext& context, Position offset)
{
    getRenderStrategy().drawPadding(context, offset, _size, _color);

    if (_child != nullptr) {
        _child->render(context, offset.add(_childPosition));
    }
}
} // ui
