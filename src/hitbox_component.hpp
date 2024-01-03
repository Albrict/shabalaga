#pragma once
#include "../include/raylib.h"
#include "../include/entt.hpp"
#include "graphics_component.hpp"
#include "sprite_component.hpp"

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
    
    inline void createHitboxInContainer(entt::registry &object_registry, HitboxComponent::Container &container, const SpriteComponent::Component &sprite, 
                        const char *slice_name, const Vector2 position)
    {
        const float scale = sprite.scale;
        Hitbox hitbox;
        hitbox.rect = sprite.sprite->getSlice(slice_name);
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

    inline void createHitboxInContainerFromSprite(entt::registry &object_registry, HitboxComponent::Container &container, const GraphicsComponent::Animation &sprite, 
                        const char *slice_name, const Vector2 position)
    {
        const float scale = sprite.scale;
        Hitbox hitbox;
        hitbox.rect = sprite.sprite->getSlice(slice_name);
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

    inline void createHitboxInContainerFromSprite(entt::registry &object_registry, HitboxComponent::Container &container, const GraphicsComponent::Sprite &sprite, 
                        const char *slice_name, const Vector2 position)
    {
        const float scale = sprite.scale;
        Hitbox hitbox;
        hitbox.rect = sprite.sprite->getSlice(slice_name);
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
