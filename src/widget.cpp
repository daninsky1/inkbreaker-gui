#include "widget.h"

#include "render_strategy.h"

namespace ui
{
namespace {
Blend2DRenderStrategy defaultRenderStrategy;
}

Size Widget::normalize(const BoxConstraints& constraint)
{
    _size = constraint.clamp(_size);
    return _size;
}

std::string Widget::toString() const { return _runtimeType; }

RenderStrategy& Widget::getRenderStrategy() const
{
    if (_renderStrategy != nullptr) {
        return *_renderStrategy;
    }
    if (_parent != nullptr) {
        return _parent->getRenderStrategy();
    }
    return defaultRenderStrategy;
}

std::string Widget::getRuntimeType()
{
    if (_runtimeType.empty()) {
        // _runtimeType = std::format("{} Widget", static_cast<void*>(this));
        _runtimeType = typeid(*this).name();
    }
    return _runtimeType;
}

SingleChildWidget& SingleChildWidget::setChild(Widget& child)
{
    _needsUpdate = true;
    _child = &child;
    _child->setParent(this);
    return *this;
}

Widget* SingleChildWidget::getChild()
{
    return _child;
}

Event& SingleChildWidget::eventHandler(Event& event)
{
    return dispatchEventToChild(event);
}

bool SingleChildWidget::hitTestChild(Position position) const
{
    if (_child == nullptr) {
        return false;
    }

    const Size childSize = _child->getSize();
    return position.x >= _childPosition.x &&
           position.y >= _childPosition.y &&
           position.x < _childPosition.x + childSize.width &&
           position.y < _childPosition.y + childSize.height;
}

Event& SingleChildWidget::dispatchEventToChild(Event& event)
{
    if (_child == nullptr || event.handled) {
        return event;
    }

    if (event.hasPosition) {
        const bool inside = hitTestChild(event.position);
        const bool wasInside = _childPointerInside;
        const bool wasActive = _childPointerActive;

        switch (event.sdlEvent.type) {
        case SDL_EVENT_MOUSE_MOTION:
            if (!inside && !wasInside && !wasActive) {
                return event;
            }
            _childPointerInside = inside;
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (!inside) {
                return event;
            }
            _childPointerInside = true;
            _childPointerActive = true;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (!inside && !wasActive) {
                return event;
            }
            _childPointerInside = inside;
            _childPointerActive = false;
            break;
        default:
            if (!inside) {
                return event;
            }
            break;
        }
    }

    const Position parentPosition = event.position;
    if (event.hasPosition) {
        event.position = {
            .x = event.position.x - _childPosition.x,
            .y = event.position.y - _childPosition.y
        };
    }

    _child->eventHandler(event);
    event.position = parentPosition;

    return event;
}
}   // ui
