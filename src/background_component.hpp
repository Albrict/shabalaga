#pragma once
#include "../include/raylib.h"
#include <filesystem>

struct BackgroundComponent {
    const Texture2D background;
    float width;
    float height;
    Vector2 offset;
    const float scroll_speed_y;
    const float scroll_speed_x;

    BackgroundComponent(const Texture2D bg_texture, const float scroll_speed_x, const float scroll_speed_y)
        : background(bg_texture),
        offset({0.f, 0.f}),
        scroll_speed_x(scroll_speed_x),
        scroll_speed_y(scroll_speed_y)
    { 
        width = background.width;
        height = background.height;
    }
};
