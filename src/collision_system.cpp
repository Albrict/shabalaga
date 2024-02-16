#include "collision_system.hpp"
#include "collision_component.hpp"
#include "../include/raylib.h"
#include "graphics.hpp"
#include "clean_up_component.hpp"

namespace CollisionSystem {
    bool checkCollisionHitboxes(const std::vector<Collision::Hitbox> &a_hitboxes, const std::vector<Collision::Hitbox> &b_hitboxes)
    {
        for (const auto &a_hitbox : a_hitboxes) {
            for (const auto &b_hitbox : b_hitboxes) {
                if (CheckCollisionRecs(a_hitbox.rect, b_hitbox.rect))
                    return true;
            }
        } 
        return false;
    }

    void checkDynamicCollisions(entt::registry &registry, const entt::entity a_entity, 
            const std::vector<Collision::Hitbox> a_hitboxes, Collision::callback a_cb)
    {
        const auto dynamic_collider_view = registry.view<Collision::DynamicComponent>();
        for (auto [b_entity, b_collider] : dynamic_collider_view.each()) {
            if (a_entity == b_entity || b_collider.visible == false)
                continue;
            const Rectangle &a_rect = registry.get<Rectangle>(a_entity);
            const Rectangle &b_rect = registry.get<Rectangle>(b_entity);
            if (registry.valid(a_entity) 
                && registry.valid(b_entity)
                && CheckCollisionRecs(a_rect, b_rect)
                && checkCollisionHitboxes(a_hitboxes, b_collider.hitboxes)) {
                    a_cb(registry, a_entity, b_entity);
            }
        }
        const auto static_collider_view = registry.view<Collision::StaticComponent>();
        for (auto [b_entity, b_collider] : static_collider_view.each()) {
            if (b_collider.visible == false)
                continue;
            const Rectangle &a_rect = registry.get<Rectangle>(a_entity);
            if (registry.valid(a_entity) 
                && registry.valid(b_entity)
                && CheckCollisionRecs(a_rect, b_collider.hitboxes[0].rect)
                && checkCollisionHitboxes(a_hitboxes, b_collider.hitboxes)) {
                    a_cb(registry, a_entity, b_entity);
            }
        }
    }

    void checkStaticCollisions(entt::registry &registry, const entt::entity a_entity, 
            const std::vector<Collision::Hitbox> a_hitboxes, Collision::callback a_cb)
    {
        const auto view = registry.view<Collision::DynamicComponent>();
        for (auto [b_entity, b_collider] : view.each()) {
            if (a_entity == b_entity || b_collider.visible == false)
                continue;
            const Rectangle &b_rect = registry.get<Rectangle>(b_entity);
            if (registry.valid(a_entity) 
                && registry.valid(b_entity)
                && CheckCollisionRecs(a_hitboxes[0].rect, b_rect)
                && checkCollisionHitboxes(a_hitboxes, b_collider.hitboxes)) {
                    a_cb(registry, a_entity, b_entity);
            }
        }
    }

    void checkBounds(Rectangle &rect)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        if (rect.x <= 0.f)
            rect.x = 0.f; 
        if (rect.x + rect.width >= resolution.x)
            rect.x = resolution.x - rect.width;
        if (rect.y <= 0.f)
            rect.y = 0.f; 
        if (rect.y + rect.height >= resolution.y)
            rect.y = resolution.y - rect.height;
    }

    void checkOutOfBounds(entt::registry &registry, const entt::entity entity, const Rectangle &rect)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        if (rect.x <= 0.f - rect.width * 2.f)
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
        else if (rect.x + rect.width >= resolution.x + rect.width * 2.f)
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
        else if (rect.y <= 0.f - rect.height * 2.f)
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
        else if (rect.y + rect.height >= resolution.y + rect.height * 2.f)
            registry.emplace_or_replace<CleanUpComponent::Component>(entity);
    }

    void checkCustomBounds(entt::registry &registry, const entt::entity entity, Collision::DynamicComponent &collider)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float custom_x = collider.custom_bound_x;
        const float custom_y = collider.custom_bound_y;
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
    auto dynamic_collider_view = registry.view<Collision::DynamicComponent>();
    for (auto [entity, dynamic_collider] : dynamic_collider_view.each()) {
        if (dynamic_collider.visible)
            checkDynamicCollisions(registry, entity, dynamic_collider.hitboxes, dynamic_collider.onCollide);
        if (registry.valid(entity)) {
            Rectangle &rect = registry.get<Rectangle>(entity);
            switch(dynamic_collider.type) {
            using enum Collision::Type;
            case BOUNDS:
                checkBounds(rect);
                break;
            case OUT_OF_BOUNDS:
                checkOutOfBounds(registry, entity, rect);
                break;
            case CUSTOM_BOUNDS:
                checkCustomBounds(registry, entity, dynamic_collider);
                break;
            case NONE:
                break;
            }
        }
    }

    auto static_collider_view = registry.view<Collision::StaticComponent>();    
    for (auto [entity, static_collider] : static_collider_view.each()) {
        if (static_collider.visible)
            checkStaticCollisions(registry, entity, static_collider.hitboxes, static_collider.onCollide);
    }
}

void CollisionSystem::updateHitboxes(entt::registry &registry)
{
    const auto &view = registry.view<Collision::DynamicComponent>(); 
    for (auto [entity, collider] : view.each()) {
        const Rectangle &rect = registry.get<Rectangle>(entity);
        for (auto &hitbox : collider.hitboxes) {
            hitbox.rect.x = rect.x + hitbox.x_padding;
            hitbox.rect.y = rect.y + hitbox.y_padding; 
        }
    }
}
