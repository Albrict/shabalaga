#pragma once
#include "sprite.hpp"
#include <memory>

namespace SpriteComponent {
    struct Component {
        std::unique_ptr<Sprite> sprite {};        
        float scale;
        bool die_when_end = false;
    };
    Component create(const std::string_view &key, const float width, const float height, const bool die_when_end = false);
};
