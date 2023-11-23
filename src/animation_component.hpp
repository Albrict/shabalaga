#pragma once
#include <vector>
#include "../include/raylib.h"

struct AnimtationComponent {
    const Texture2D texture;
    const std::vector<Rectangle> source_rectangles;
    const int fps;
    int current_frame = 0;

    AnimtationComponent(const Texture2D spritesheet, const std::vector<Rectangle> &src_rects, const int fps)
        : texture(spritesheet), 
        source_rectangles(src_rects),
        fps(fps) {}
};
