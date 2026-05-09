#include "render_strategy.h"

#include <algorithm>

namespace ui {

Size NullRenderStrategy::measureText(const std::string& value, int32_t fontSize, const std::string& fontFilepath)
{
    (void)fontFilepath;

    int32_t maxLineWidth = 0;
    int32_t lineCount = 1;
    int32_t currentLineWidth = 0;

    for (char character : value) {
        if (character == '\n') {
            maxLineWidth = std::max(maxLineWidth, currentLineWidth);
            currentLineWidth = 0;
            ++lineCount;
            continue;
        }
        currentLineWidth += std::max(1, fontSize / 2);
    }

    maxLineWidth = std::max(maxLineWidth, currentLineWidth);
    return {
        .width = maxLineWidth,
        .height = std::max(1, lineCount) * std::max(1, fontSize)
    };
}

void NullRenderStrategy::drawWindow(const Size& size, Color color) { (void)size; (void)color; }
void NullRenderStrategy::drawContainer(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderStrategy::drawFlexContainer(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderStrategy::drawCenter(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderStrategy::drawAlign(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderStrategy::drawPadding(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderStrategy::drawGrid(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderStrategy::drawButton(Position offset, const Size& size, Color backgroundColor, Color borderColor)
{
    (void)offset;
    (void)size;
    (void)backgroundColor;
    (void)borderColor;
}

void NullRenderStrategy::drawText(
    Position offset,
    const Size& size,
    const std::string& value,
    Color textColor,
    Color backgroundColor,
    int32_t fontSize,
    const std::string& fontFilepath,
    int32_t horizontalAlignment
)
{
    (void)offset;
    (void)size;
    (void)value;
    (void)textColor;
    (void)backgroundColor;
    (void)fontSize;
    (void)fontFilepath;
    (void)horizontalAlignment;
}

void NullRenderStrategy::drawConstrainedBox(Position offset, const Size& size) { (void)offset; (void)size; }
void NullRenderStrategy::drawUnconstrainedBox(Position offset, const Size& size) { (void)offset; (void)size; }
void NullRenderStrategy::drawLimitedBox(Position offset, const Size& size) { (void)offset; (void)size; }
void NullRenderStrategy::drawOverlay(Position offset, const Size& size) { (void)offset; (void)size; }
void NullRenderStrategy::drawCalculator(Position offset, const Size& size) { (void)offset; (void)size; }

} // namespace ui
