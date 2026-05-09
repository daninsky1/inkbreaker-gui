#pragma once

#include "widget.h"

#include "layout.h"
#include "padding.h"

namespace ui
{

/*
* Widget container class.
* This class is intended to hold common positioning and sizing of widgets.
*/
class Container : public SingleChildWidget
{
public:
    Container()
        : _margin(0.0), _padding(0, 0, 0, 0) { }
    
    void setColor(Color color) { _backgroundColor = color; }
    Color getColor() const { return _backgroundColor; }
    void setMargin(const css::Margin& margin);
    const css::Margin& getMargin() const;
    void setPadding(const BoxSpace padding);
    BoxSpace getPadding() const;

    Size layout(const BoxConstraints& constraint) override;
    void render(Position offset) override;
private:
    css::Margin _margin;
    BoxSpace _padding;
    Color _backgroundColor = Colors::GRAY;
};

}
