#pragma once
#include "../include/raylib.h"
#include "../include/entt.hpp"
#include "resource_system.hpp"

namespace HitboxComponent {
    struct Hitbox {
        Rectangle rect;
        std::string_view name;
        float x_padding;
        float y_padding;
    };
    struct Container {
        entt::entity parent_object = entt::null;
        std::vector<Hitbox> hitboxes {};
    };
    
    inline void createHitboxInContainerFromAseprite(entt::registry &object_registry, HitboxComponent::Container &container, const std::string_view &key, 
                        const unsigned int slice_id, const Vector2 position, const float scale)
    {
        Hitbox hitbox;
        hitbox.rect = ResourceSystem::getAsepriteSlice(key, slice_id).bounds;
        hitbox.rect.x /= scale;
        hitbox.rect.y /= scale;
        hitbox.rect.height /= scale;
        hitbox.rect.width /= scale;
            
        hitbox.x_padding = hitbox.rect.x;
        hitbox.y_padding = hitbox.rect.y;
        
        hitbox.rect.x += position.x;
        hitbox.rect.y += position.y;

        container.hitboxes.push_back(hitbox);
    }
}
