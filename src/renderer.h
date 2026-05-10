#pragma once

#include <cstdint>
#include <string>
#include <type_traits>

#include "layout.h"
#include "render_types.h"

namespace ui {

class Renderer final {
public:
    Renderer();

    template <typename T>
    explicit Renderer(T& renderer)
    {
        bind(renderer);
    }

    template <typename T>
    void bind(T& renderer)
    {
        using RendererType = std::remove_cvref_t<T>;
        _self = &renderer;
        _ops = &opsFor<RendererType>();
    }

    Size measureText(const std::string& value, int32_t fontSize, const std::string& fontFilepath);

    void drawWindow(const Size& size, Color color);
    void drawContainer(Position offset, const Size& size, Color color);
    void drawFlexContainer(Position offset, const Size& size, Color color);
    void drawCenter(Position offset, const Size& size, Color color);
    void drawAlign(Position offset, const Size& size, Color color);
    void drawPadding(Position offset, const Size& size, Color color);
    void drawGrid(Position offset, const Size& size, Color color);
    void drawButton(Position offset, const Size& size, Color backgroundColor, Color borderColor);
    void drawText(
        Position offset,
        const Size& size,
        const std::string& value,
        Color textColor,
        Color backgroundColor,
        int32_t fontSize,
        const std::string& fontFilepath,
        int32_t horizontalAlignment
    );
    void drawConstrainedBox(Position offset, const Size& size);
    void drawUnconstrainedBox(Position offset, const Size& size);
    void drawLimitedBox(Position offset, const Size& size);
    void drawOverlay(Position offset, const Size& size);
    void drawCalculator(Position offset, const Size& size);

private:
    struct Ops {
        Size (*measureText)(void*, const std::string&, int32_t, const std::string&);
        void (*drawWindow)(void*, const Size&, Color);
        void (*drawContainer)(void*, Position, const Size&, Color);
        void (*drawFlexContainer)(void*, Position, const Size&, Color);
        void (*drawCenter)(void*, Position, const Size&, Color);
        void (*drawAlign)(void*, Position, const Size&, Color);
        void (*drawPadding)(void*, Position, const Size&, Color);
        void (*drawGrid)(void*, Position, const Size&, Color);
        void (*drawButton)(void*, Position, const Size&, Color, Color);
        void (*drawText)(void*, Position, const Size&, const std::string&, Color, Color, int32_t, const std::string&, int32_t);
        void (*drawConstrainedBox)(void*, Position, const Size&);
        void (*drawUnconstrainedBox)(void*, Position, const Size&);
        void (*drawLimitedBox)(void*, Position, const Size&);
        void (*drawOverlay)(void*, Position, const Size&);
        void (*drawCalculator)(void*, Position, const Size&);
    };

    template <typename T>
    static const Ops& opsFor()
    {
        static const Ops ops = {
            .measureText = [](void* self, const std::string& value, int32_t fontSize, const std::string& fontFilepath) {
                return static_cast<T*>(self)->measureText(value, fontSize, fontFilepath);
            },
            .drawWindow = [](void* self, const Size& size, Color color) {
                static_cast<T*>(self)->drawWindow(size, color);
            },
            .drawContainer = [](void* self, Position offset, const Size& size, Color color) {
                static_cast<T*>(self)->drawContainer(offset, size, color);
            },
            .drawFlexContainer = [](void* self, Position offset, const Size& size, Color color) {
                static_cast<T*>(self)->drawFlexContainer(offset, size, color);
            },
            .drawCenter = [](void* self, Position offset, const Size& size, Color color) {
                static_cast<T*>(self)->drawCenter(offset, size, color);
            },
            .drawAlign = [](void* self, Position offset, const Size& size, Color color) {
                static_cast<T*>(self)->drawAlign(offset, size, color);
            },
            .drawPadding = [](void* self, Position offset, const Size& size, Color color) {
                static_cast<T*>(self)->drawPadding(offset, size, color);
            },
            .drawGrid = [](void* self, Position offset, const Size& size, Color color) {
                static_cast<T*>(self)->drawGrid(offset, size, color);
            },
            .drawButton = [](void* self, Position offset, const Size& size, Color backgroundColor, Color borderColor) {
                static_cast<T*>(self)->drawButton(offset, size, backgroundColor, borderColor);
            },
            .drawText = [](void* self, Position offset, const Size& size, const std::string& value, Color textColor, Color backgroundColor, int32_t fontSize, const std::string& fontFilepath, int32_t horizontalAlignment) {
                static_cast<T*>(self)->drawText(offset, size, value, textColor, backgroundColor, fontSize, fontFilepath, horizontalAlignment);
            },
            .drawConstrainedBox = [](void* self, Position offset, const Size& size) {
                static_cast<T*>(self)->drawConstrainedBox(offset, size);
            },
            .drawUnconstrainedBox = [](void* self, Position offset, const Size& size) {
                static_cast<T*>(self)->drawUnconstrainedBox(offset, size);
            },
            .drawLimitedBox = [](void* self, Position offset, const Size& size) {
                static_cast<T*>(self)->drawLimitedBox(offset, size);
            },
            .drawOverlay = [](void* self, Position offset, const Size& size) {
                static_cast<T*>(self)->drawOverlay(offset, size);
            },
            .drawCalculator = [](void* self, Position offset, const Size& size) {
                static_cast<T*>(self)->drawCalculator(offset, size);
            },
        };
        return ops;
    }

    void* _self = nullptr;
    const Ops* _ops = nullptr;
};

class NullRenderer final {
public:
    Size measureText(const std::string& value, int32_t fontSize, const std::string& fontFilepath);

    void drawWindow(const Size& size, Color color);
    void drawContainer(Position offset, const Size& size, Color color);
    void drawFlexContainer(Position offset, const Size& size, Color color);
    void drawCenter(Position offset, const Size& size, Color color);
    void drawAlign(Position offset, const Size& size, Color color);
    void drawPadding(Position offset, const Size& size, Color color);
    void drawGrid(Position offset, const Size& size, Color color);
    void drawButton(Position offset, const Size& size, Color backgroundColor, Color borderColor);
    void drawText(
        Position offset,
        const Size& size,
        const std::string& value,
        Color textColor,
        Color backgroundColor,
        int32_t fontSize,
        const std::string& fontFilepath,
        int32_t horizontalAlignment
    );
    void drawConstrainedBox(Position offset, const Size& size);
    void drawUnconstrainedBox(Position offset, const Size& size);
    void drawLimitedBox(Position offset, const Size& size);
    void drawOverlay(Position offset, const Size& size);
    void drawCalculator(Position offset, const Size& size);
};

} // namespace ui
