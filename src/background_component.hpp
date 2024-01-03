#pragma once
#include "../include/raylib.h"
#include "../include/entt.hpp"


namespace BackgroundComponent {
    struct Background {
        Texture2D background;
        float width;
        float height;
        float scroll_speed_x;
        float scroll_speed_y;
        Vector2 offset;
    };
    
    entt::entity create(entt::registry &object_registry, const Texture2D texture, const float scroll_speed_y = 0.f, const float scroll_speed_x = 0.f);
}
