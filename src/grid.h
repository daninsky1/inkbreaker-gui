#pragma once

#include <utility>
#include <vector>

#include "widget.h"

namespace ui {

class Grid : public MultiChildWidget {
public:
    struct Cell {
        int32_t column = 0;
        int32_t row = 0;
        int32_t columnSpan = 1;
        int32_t rowSpan = 1;
    };

    Grid(int32_t columns, int32_t rows);

    void setGap(int32_t gap) { _rowGap = gap; _columnGap = gap; }
    void setRowGap(int32_t gap) { _rowGap = gap; }
    void setColumnGap(int32_t gap) { _columnGap = gap; }
    void setPadding(BoxSpace padding) { _padding = padding; }
    void setColor(BLRgba32 color) { _color = color; }
    void setCells(std::vector<Cell> cells) { _cells = std::move(cells); }

    Size layout(const BoxConstraints& boxConstraints) override;
    void render(BLContext& context, Position offset) override;
    Event& eventHandler(Event& event) override;

private:
    [[nodiscard]] int32_t contentWidth() const;
    [[nodiscard]] int32_t contentHeight() const;

    int32_t _columns = 1;
    int32_t _rows = 1;
    int32_t _rowGap = 0;
    int32_t _columnGap = 0;
    BoxSpace _padding{};
    BLRgba32 _color = Colors::TRANSPARENT;
    std::vector<Position> _childPositions;
    std::vector<bool> _childPointerInside;
    std::vector<bool> _childPointerActive;
    std::vector<Cell> _cells;
};

} // namespace ui
