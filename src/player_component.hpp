#pragma once
#include "animation_component.hpp"
#include "../include/raylib-aseprite.hpp"
#include <array>

struct PlayerComponent {
    Texture2D texture;
    PlayerComponent(const Texture2D texture) 
        : texture(texture) {}
};
