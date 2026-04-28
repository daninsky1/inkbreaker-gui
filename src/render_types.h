#pragma once

#include <blend2d.h>

namespace ui::Colors {
inline constexpr BLRgba32 TRANSPARENT{  0,   0,   0,   0};
inline constexpr BLRgba32 BLACK      {  0,   0,   0, 255};
inline constexpr BLRgba32 DKGRAY     { 64,  64,  64, 255};
inline constexpr BLRgba32 GRAY       {128, 128, 128, 255};
inline constexpr BLRgba32 LTGRAY     {191, 191, 191, 255};
inline constexpr BLRgba32 WHITE      {255, 255, 255, 255};
inline constexpr BLRgba32 RED        {255,   0,   0, 255};
inline constexpr BLRgba32 GREEN      {  0, 255,   0, 255};
inline constexpr BLRgba32 BLUE       {  0,   0, 255, 255};
inline constexpr BLRgba32 YELLOW     {255, 255,   0, 255};
inline constexpr BLRgba32 CYAN       {  0, 255, 255, 255};
inline constexpr BLRgba32 MAGENTA    {255,   0, 255, 255};
}
