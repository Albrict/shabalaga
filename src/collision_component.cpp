#include "collision_component.hpp"
#include "aseprite.hpp"
#include "resource_system.hpp"

namespace Collision {
    inline void loadHitboxes(std::vector<Hitbox> &hitboxes, const std::string_view &key, const Rectangle rect)
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

            hitboxes.push_back(hitbox);
        } 
    }
}

void Collision::addDynamicCollisionFromAseprite(entt::registry &registry, const entt::entity entity, 
        const std::string_view &key, const bool visibile, const Type type, callback onCollide)
{
    const Rectangle rect = registry.get<Rectangle>(entity);
    // Init struct
    DynamicComponent collider = {
        .visible = visibile, 
        .type = type,
        .onCollide = onCollide
    };

    // Load hitboxes from aseprite
    loadHitboxes(collider.hitboxes, key, rect);
    registry.emplace_or_replace<DynamicComponent>(entity, collider);
}

void Collision::addDynamicCollisionFromAseprite(entt::registry &registry, const entt::entity entity, 
        const std::string_view &key, const bool visibile, 
        const float custom_bound_x, const float custom_bound_y, callback onCollide)
{
    const Rectangle rect = registry.get<Rectangle>(entity);
    // Init struct
    DynamicComponent collider = {
        .visible = visibile, 
        .custom_bound_x = custom_bound_x,
        .custom_bound_y = custom_bound_y,
        .type = Type::CUSTOM_BOUNDS,
        .onCollide = onCollide
    };

    // Load hitboxes from aseprite
    loadHitboxes(collider.hitboxes, key, rect);
    registry.emplace_or_replace<DynamicComponent>(entity, collider);
}

void Collision::addStaticCollisionFromAseprite(entt::registry &registry, const entt::entity entity, 
        const std::string_view &key, bool visibile, callback onCollide)
{
    const Rectangle rect = registry.get<Rectangle>(entity);
    StaticComponent collider = {
        .visible = visibile,
        .onCollide = onCollide
    };
    loadHitboxes(collider.hitboxes, key, rect);
    registry.emplace<StaticComponent>(entity, collider);
}

void Collision::addStaticCollision(entt::registry &registry, const entt::entity entity,
        const std::string_view &key, std::vector<Hitbox> hitboxes, bool visibile, callback onCollide)
{
    StaticComponent collider = {
        .visible = visibile,
        .hitboxes = hitboxes,
        .onCollide = onCollide
    };
    registry.emplace<StaticComponent>(entity, collider);
}
