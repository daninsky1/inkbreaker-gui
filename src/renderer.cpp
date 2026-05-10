#include "renderer.h"

#include <algorithm>

namespace ui {

namespace {
NullRenderer fallbackRenderer;
}

Renderer::Renderer()
{
    bind(fallbackRenderer);
}

Size Renderer::measureText(const std::string& value, int32_t fontSize, const std::string& fontFilepath)
{
    return _ops->measureText(_self, value, fontSize, fontFilepath);
}

void Renderer::drawWindow(const Size& size, Color color) { _ops->drawWindow(_self, size, color); }
void Renderer::drawContainer(Position offset, const Size& size, Color color) { _ops->drawContainer(_self, offset, size, color); }
void Renderer::drawFlexContainer(Position offset, const Size& size, Color color) { _ops->drawFlexContainer(_self, offset, size, color); }
void Renderer::drawCenter(Position offset, const Size& size, Color color) { _ops->drawCenter(_self, offset, size, color); }
void Renderer::drawAlign(Position offset, const Size& size, Color color) { _ops->drawAlign(_self, offset, size, color); }
void Renderer::drawPadding(Position offset, const Size& size, Color color) { _ops->drawPadding(_self, offset, size, color); }
void Renderer::drawGrid(Position offset, const Size& size, Color color) { _ops->drawGrid(_self, offset, size, color); }
void Renderer::drawButton(Position offset, const Size& size, Color backgroundColor, Color borderColor)
{
    _ops->drawButton(_self, offset, size, backgroundColor, borderColor);
}

void Renderer::drawText(
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
    _ops->drawText(_self, offset, size, value, textColor, backgroundColor, fontSize, fontFilepath, horizontalAlignment);
}

void Renderer::drawConstrainedBox(Position offset, const Size& size) { _ops->drawConstrainedBox(_self, offset, size); }
void Renderer::drawUnconstrainedBox(Position offset, const Size& size) { _ops->drawUnconstrainedBox(_self, offset, size); }
void Renderer::drawLimitedBox(Position offset, const Size& size) { _ops->drawLimitedBox(_self, offset, size); }
void Renderer::drawOverlay(Position offset, const Size& size) { _ops->drawOverlay(_self, offset, size); }
void Renderer::drawCalculator(Position offset, const Size& size) { _ops->drawCalculator(_self, offset, size); }

Size NullRenderer::measureText(const std::string& value, int32_t fontSize, const std::string& fontFilepath)
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

void NullRenderer::drawWindow(const Size& size, Color color) { (void)size; (void)color; }
void NullRenderer::drawContainer(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderer::drawFlexContainer(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderer::drawCenter(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderer::drawAlign(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderer::drawPadding(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderer::drawGrid(Position offset, const Size& size, Color color) { (void)offset; (void)size; (void)color; }
void NullRenderer::drawButton(Position offset, const Size& size, Color backgroundColor, Color borderColor)
{
    (void)offset;
    (void)size;
    (void)backgroundColor;
    (void)borderColor;
}

void NullRenderer::drawText(
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

void NullRenderer::drawConstrainedBox(Position offset, const Size& size) { (void)offset; (void)size; }
void NullRenderer::drawUnconstrainedBox(Position offset, const Size& size) { (void)offset; (void)size; }
void NullRenderer::drawLimitedBox(Position offset, const Size& size) { (void)offset; (void)size; }
void NullRenderer::drawOverlay(Position offset, const Size& size) { (void)offset; (void)size; }
void NullRenderer::drawCalculator(Position offset, const Size& size) { (void)offset; (void)size; }

} // namespace ui
