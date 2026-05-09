#include "grid.h"

#include <algorithm>

#include "render_strategy.h"

namespace ui {

Grid::Grid(int32_t columns, int32_t rows)
    : _columns(std::max(1, columns)), _rows(std::max(1, rows))
{
}

Size Grid::layout(const BoxConstraints& boxConstraints)
{
    _size = {
        .width = boxConstraints.maxWidth,
        .height = boxConstraints.maxHeight
    };

    const int32_t availableWidth = std::max(0, contentWidth() - _columnGap * (_columns - 1));
    const int32_t availableHeight = std::max(0, contentHeight() - _rowGap * (_rows - 1));
    const int32_t cellWidth = _columns > 0 ? availableWidth / _columns : 0;
    const int32_t cellHeight = _rows > 0 ? availableHeight / _rows : 0;

    _childPositions.assign(_children.size(), Position{});
    _childPointerInside.resize(_children.size(), false);
    _childPointerActive.resize(_children.size(), false);

    for (size_t index = 0; index < _children.size(); ++index) {
        Widget* child = _children[index];
        if (child == nullptr) {
            continue;
        }

        Cell cell = {
            .column = static_cast<int32_t>(index) % _columns,
            .row = static_cast<int32_t>(index) / _columns,
            .columnSpan = 1,
            .rowSpan = 1
        };
        if (index < _cells.size()) {
            cell = _cells[index];
        }

        cell.column = std::clamp(cell.column, 0, _columns - 1);
        cell.row = std::clamp(cell.row, 0, _rows - 1);
        cell.columnSpan = std::clamp(cell.columnSpan, 1, _columns - cell.column);
        cell.rowSpan = std::clamp(cell.rowSpan, 1, _rows - cell.row);

        if (cell.row >= _rows) {
            child->layout({0, 0, 0, 0});
            continue;
        }

        _childPositions[index] = {
            .x = _padding.left + cell.column * (cellWidth + _columnGap),
            .y = _padding.top + cell.row * (cellHeight + _rowGap)
        };

        const int32_t childWidth = cellWidth * cell.columnSpan + _columnGap * (cell.columnSpan - 1);
        const int32_t childHeight = cellHeight * cell.rowSpan + _rowGap * (cell.rowSpan - 1);

        child->layout({
            .minWidth = childWidth,
            .minHeight = childHeight,
            .maxWidth = childWidth,
            .maxHeight = childHeight
        });
    }

    return normalize(boxConstraints);
}

void Grid::render(Position offset)
{
    getRenderStrategy().drawGrid(offset, _size, _color);

    for (size_t index = 0; index < _children.size(); ++index) {
        if (_children[index] != nullptr && index < _childPositions.size()) {
            _children[index]->render(offset.add(_childPositions[index]));
        }
    }
}

Event& Grid::eventHandler(Event& event)
{
    if (!event.hasPosition) {
        for (auto* child : _children) {
            if (child != nullptr && !event.handled) {
                child->eventHandler(event);
            }
        }
        return event;
    }

    for (size_t reverseIndex = _children.size(); reverseIndex > 0; --reverseIndex) {
        const size_t index = reverseIndex - 1;
        if (_children[index] == nullptr || event.handled || index >= _childPositions.size()) {
            continue;
        }

        const Position childPosition = _childPositions[index];
        const Size childSize = _children[index]->getSize();
        const bool inside = event.position.x >= childPosition.x &&
                            event.position.y >= childPosition.y &&
                            event.position.x < childPosition.x + childSize.width &&
                            event.position.y < childPosition.y + childSize.height;
        bool shouldDispatch = inside;
        if (event.sdlEvent.type == SDL_EVENT_MOUSE_MOTION) {
            shouldDispatch = inside || _childPointerInside[index] || _childPointerActive[index];
            _childPointerInside[index] = inside;
        } else if (event.sdlEvent.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            if (inside) {
                _childPointerInside[index] = true;
                _childPointerActive[index] = true;
            }
        } else if (event.sdlEvent.type == SDL_EVENT_MOUSE_BUTTON_UP) {
            shouldDispatch = inside || _childPointerActive[index];
            _childPointerInside[index] = inside;
            _childPointerActive[index] = false;
        }

        if (!shouldDispatch) {
            continue;
        }

        const Position parentPosition = event.position;
        event.position = {
            .x = event.position.x - childPosition.x,
            .y = event.position.y - childPosition.y
        };
        _children[index]->eventHandler(event);
        event.position = parentPosition;
    }

    return event;
}

int32_t Grid::contentWidth() const
{
    return std::max(0, _size.width - _padding.left - _padding.right);
}

int32_t Grid::contentHeight() const
{
    return std::max(0, _size.height - _padding.top - _padding.bottom);
}

} // namespace ui
