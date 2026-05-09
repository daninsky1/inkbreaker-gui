#pragma once

#include <cstdint>
#include <string>

#include "layout.h"
#include "render_types.h"

namespace ui {

class RenderStrategy {
public:
    virtual ~RenderStrategy() = default;

    virtual Size measureText(const std::string& value, int32_t fontSize, const std::string& fontFilepath) = 0;

    virtual void drawWindow(const Size& size, Color color) = 0;
    virtual void drawContainer(Position offset, const Size& size, Color color) = 0;
    virtual void drawFlexContainer(Position offset, const Size& size, Color color) = 0;
    virtual void drawCenter(Position offset, const Size& size, Color color) = 0;
    virtual void drawAlign(Position offset, const Size& size, Color color) = 0;
    virtual void drawPadding(Position offset, const Size& size, Color color) = 0;
    virtual void drawGrid(Position offset, const Size& size, Color color) = 0;
    virtual void drawButton(Position offset, const Size& size, Color backgroundColor, Color borderColor) = 0;
    virtual void drawText(
        Position offset,
        const Size& size,
        const std::string& value,
        Color textColor,
        Color backgroundColor,
        int32_t fontSize,
        const std::string& fontFilepath,
        int32_t horizontalAlignment
    ) = 0;
    virtual void drawConstrainedBox(Position offset, const Size& size) = 0;
    virtual void drawUnconstrainedBox(Position offset, const Size& size) = 0;
    virtual void drawLimitedBox(Position offset, const Size& size) = 0;
    virtual void drawOverlay(Position offset, const Size& size) = 0;
    virtual void drawCalculator(Position offset, const Size& size) = 0;
};

class NullRenderStrategy final : public RenderStrategy {
public:
    Size measureText(const std::string& value, int32_t fontSize, const std::string& fontFilepath) override;

    void drawWindow(const Size& size, Color color) override;
    void drawContainer(Position offset, const Size& size, Color color) override;
    void drawFlexContainer(Position offset, const Size& size, Color color) override;
    void drawCenter(Position offset, const Size& size, Color color) override;
    void drawAlign(Position offset, const Size& size, Color color) override;
    void drawPadding(Position offset, const Size& size, Color color) override;
    void drawGrid(Position offset, const Size& size, Color color) override;
    void drawButton(Position offset, const Size& size, Color backgroundColor, Color borderColor) override;
    void drawText(
        Position offset,
        const Size& size,
        const std::string& value,
        Color textColor,
        Color backgroundColor,
        int32_t fontSize,
        const std::string& fontFilepath,
        int32_t horizontalAlignment
    ) override;
    void drawConstrainedBox(Position offset, const Size& size) override;
    void drawUnconstrainedBox(Position offset, const Size& size) override;
    void drawLimitedBox(Position offset, const Size& size) override;
    void drawOverlay(Position offset, const Size& size) override;
    void drawCalculator(Position offset, const Size& size) override;
};

} // namespace ui
