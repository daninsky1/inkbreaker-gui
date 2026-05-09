//
// Created by daniel on 29/07/2025.
//

#pragma once
#include "widget.h"

namespace ui {

class ConstrainedBox : public SingleChildWidget {
public:
    ConstrainedBox(BoxConstraints boxConstraints, Widget* child) {
        _boxConstraints = boxConstraints;
        if (child != nullptr) {
            setChild(*child);
        }
    }

    Size layout(const BoxConstraints& boxConstraints);
    void render(Position offset);

protected:

};

} // ui
