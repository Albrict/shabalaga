#pragma once
#include "../include/raylib.h"
#include <filesystem>

struct BackgroundComponent {
    Texture2D background {};
    float width;
    float height;
    Vector2 offset;
    const float scroll_speed_y;
    const float scroll_speed_x;
    BackgroundComponent(const std::filesystem::path &path, const float scroll_speed_x, const float scroll_speed_y)
        : offset({0.f, 0.f}),
        scroll_speed_x(scroll_speed_x),
        scroll_speed_y(scroll_speed_y)
    { 
        background = LoadTexture(path.c_str()); 
        width = background.width;
        height = background.height;
    }
};
