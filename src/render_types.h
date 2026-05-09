#pragma once

#include <cstdint>

namespace ui {

struct Color {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 255;
};

namespace Colors {
inline constexpr Color TRANSPARENT{  0,   0,   0,   0};
inline constexpr Color BLACK      {  0,   0,   0, 255};
inline constexpr Color DKGRAY     { 64,  64,  64, 255};
inline constexpr Color GRAY       {128, 128, 128, 255};
inline constexpr Color LTGRAY     {191, 191, 191, 255};
inline constexpr Color WHITE      {255, 255, 255, 255};
inline constexpr Color RED        {255,   0,   0, 255};
inline constexpr Color GREEN      {  0, 255,   0, 255};
inline constexpr Color BLUE       {  0,   0, 255, 255};
inline constexpr Color YELLOW     {255, 255,   0, 255};
inline constexpr Color CYAN       {  0, 255, 255, 255};
inline constexpr Color MAGENTA    {255,   0, 255, 255};
} // namespace Colors

} // namespace ui
