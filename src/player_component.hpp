#pragma once
#include "aseprite_wrapper.hpp"
#include <array>
#include <memory>

struct PlayerComponent {
    std::shared_ptr<AsepriteWrapper> sprite;
    PlayerComponent(std::shared_ptr<AsepriteWrapper> sprite) 
        : sprite(sprite) {}
};
