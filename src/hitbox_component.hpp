#pragma once
#include "../include/raylib.h"
#include "../include/entt.hpp"
#include "aseprite.hpp"
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
    
    inline void loadHitboxesInContainer(HitboxComponent::Container &container, const std::string_view &key, const Rectangle rect)
    {
        const Aseprite::Aseprite aseprite = ResourceSystem::getAseprite(key);
        const float sprite_width = Aseprite::GetAsepriteWidth(aseprite);
        const float sprite_height = Aseprite::GetAsepriteHeight(aseprite);
        const float scale = std::min(sprite_width / rect.width, sprite_height / rect.height);

        for (size_t i = 0; i < Aseprite::GetAsepriteSliceCount(aseprite); ++i) {
            Hitbox hitbox;
            hitbox.rect = ResourceSystem::getAsepriteSlice(key, i).bounds;
            hitbox.rect.x /= scale;
            hitbox.rect.y /= scale;
            hitbox.rect.height /= scale;
            hitbox.rect.width /= scale;
                
            hitbox.x_padding = hitbox.rect.x;
            hitbox.y_padding = hitbox.rect.y;
            
            hitbox.rect.x += rect.x;
            hitbox.rect.y += rect.y;

            container.hitboxes.push_back(hitbox);
        } 
    }
}
