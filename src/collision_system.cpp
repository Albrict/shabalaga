#include "collision_system.hpp"
#include "collison_component.hpp"
#include "../include/raylib.h"
#include "graphics.hpp"
#include "hitbox_component.hpp"
#include "clean_up_component.hpp"

namespace CollisionSystem {
    bool checkCollisionHitboxes(const HitboxComponent::Container &a_container, const HitboxComponent::Container &b_container)
    {
        for (const auto &a_hitbox : a_container.hitboxes) {
            for (const auto &b_hitbox : b_container.hitboxes) {
                if (CheckCollisionRecs(a_hitbox.rect, b_hitbox.rect))
                    return true;
            }
        } 
        return false;
    }

    void checkCollisionBetweenObjects(entt::registry &registry, const entt::entity a_entity, const CollisionComponent::Component &a_collider)
    {
        const auto view = registry.view<CollisionComponent::Component>();
        for (auto [b_entity, b_collider] : view.each()) {
            if (a_entity == b_entity || b_collider.visible == false)
                continue;
            if (registry.valid(a_entity) && registry.valid(b_entity)) {
                const Rectangle &a_rect = registry.get<Rectangle>(a_entity);
                const Rectangle &b_rect = registry.get<Rectangle>(b_entity);
                if (CheckCollisionRecs(a_rect, b_rect)) {
                    const auto &a_container = registry.get<HitboxComponent::Container>(a_entity);
                    const auto &b_container = registry.get<HitboxComponent::Container>(b_entity);
                    
                    if (checkCollisionHitboxes(a_container, b_container)) {
                        if (a_collider.onCollide)
                            a_collider.onCollide(registry, a_entity, b_entity);
                    }
                }
            }
        }
    }

    void checkBounds(entt::registry &registry, const entt::entity entity)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        Rectangle &rect = registry.get<Rectangle>(entity);
        
        if (rect.x <= 0.f)
            rect.x = 0.f; 
        if (rect.x + rect.width >= resolution.x)
            rect.x = resolution.x - rect.width;
        if (rect.y <= 0.f)
            rect.y = 0.f; 
        if (rect.y + rect.height >= resolution.y)
            rect.y = resolution.y - rect.height;
    }

    void checkOutOfBounds(entt::registry &registry, const entt::entity entity)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        Rectangle &rect = registry.get<Rectangle>(entity);
        if (rect.x <= 0.f - rect.width * 2.f)
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
        else if (rect.x + rect.width >= resolution.x + rect.width * 2.f)
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
        else if (rect.y <= 0.f - rect.height * 2.f)
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
        else if (rect.y + rect.height >= resolution.y + rect.height * 2.f)
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
    }

    void checkCustomBounds(entt::registry &registry, const entt::entity entity, const float custom_x, const float custom_y)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        Rectangle &rect = registry.get<Rectangle>(entity);
        if (rect.x <= -custom_x)
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
        else if (rect.x + rect.width >= resolution.x + custom_x)
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
        else if (rect.y <= -custom_y)
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
        else if (rect.y + rect.height >= resolution.y + custom_y) 
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
    }
}

void CollisionSystem::update(entt::registry &registry)
{
    auto view = registry.view<CollisionComponent::Component>();
    for (auto [entity, collider] : view.each()) {
        if (collider.visible)
            checkCollisionBetweenObjects(registry, entity, collider);
        if (registry.valid(entity)) {
            switch(collider.type) {
            using enum CollisionComponent::Type;
            case BOUNDS:
                checkBounds(registry, entity);
                break;
            case OUT_OF_BOUNDS:
                checkOutOfBounds(registry, entity);
                break;
            case CUSTOM_BOUNDS:
                checkCustomBounds(registry, entity, collider.custom_bound_x, collider.custom_bound_y);
                break;
            case NONE:
                break;
            }
        }
    }
}
