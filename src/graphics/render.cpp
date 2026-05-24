#include "render.h"

namespace gfx {

Shape& Shape::operator=(const Shape& shape)
{
    if (this != &shape) {
        _pimpl = shape._pimpl->clone();
    }
    return *this;
}

} // namespace gfx
