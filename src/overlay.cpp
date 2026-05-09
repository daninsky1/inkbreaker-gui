#include "overlay.h"

#include "render_strategy.h"

namespace ui
{

Overlay::Overlay(const std::vector<Widget*>& children)
{
    setChildren(children);
}

Size Overlay::layout(const BoxConstraints& boxConstraints)
{
    _size = {
        .width = boxConstraints.maxWidth,
        .height = boxConstraints.maxHeight
    };

    for (auto* child : _children) {
        if (child != nullptr) {
            child->layout(boxConstraints);
        }
    }

    return normalize(boxConstraints);
}

void Overlay::render(BLContext& context, Position offset)
{
    getRenderStrategy().drawOverlay(context, offset, _size);

    for (auto* child : _children) {
        if (child != nullptr) {
            child->render(context, offset);
        }
    }
}

Event& Overlay::eventHandler(Event& event)
{
    for (auto it = _children.rbegin(); it != _children.rend(); ++it) {
        if (*it == nullptr || event.handled) {
            continue;
        }

        (*it)->eventHandler(event);
    }

    return event;
}

} // namespace ui
