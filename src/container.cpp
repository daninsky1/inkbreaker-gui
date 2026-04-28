#include "container.h"


namespace ui
{
void Container::setMargin(const css::Margin& margin)
{
    _margin = margin;
}
const css::Margin& Container::getMargin() const
{
    return _margin;
}
void Container::setPadding(const BoxSpace padding)
{
    _padding = padding;
}
BoxSpace Container::getPadding() const
{
    return _padding;
}

Size Container::layout(const BoxConstraints& constraint)
{
    if (_child != nullptr) {
        _childPosition = { .x = _padding.left, .y = _padding.top };

        // Assumes the size of the child
        _size = _child->layout(constraint);
    } else if (!_size.hasSize()) {
        // Assumes the max size of the parent, this can lead to a bug, if later
        // be decided to implements the unrestricted widget, there will be no
        // reasable max size to get from constraint
        _size = {
            std::max(constraint.minWidth, constraint.maxWidth),
            std::max(constraint.minWidth, constraint.maxHeight)
        };
    }
    // Implementação de padding dentro do container
    _size.width += (_padding.left + _padding.right);
    _size.height += (_padding.top + _padding.bottom);

    return normalize(constraint);
}

void Container::render(BLContext& context, Position offset)
{
    context.save();

    context.translate(offset.x, offset.y);

    // Define o retângulo de clipping do container
    context.clipToRect(BLRectI{0, 0, _size.width, _size.height});
    context.fillRect(BLRectI{0, 0, _size.width, _size.height}, _backgroundColor);

    // Restaura o estado anterior do renderer
    context.restore();

    if (_child != nullptr) {
        _child->render(context, offset.add(_childPosition));
    }
}
} // namespace ui
