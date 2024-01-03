#include "bullet_entity.hpp"
#include "clean_up_component.hpp"
#include "collison_component.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
#include "object_component.hpp"
#include "small_explosion_entity.hpp"

namespace BulletEntity {
    void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity) 
    {
        const auto type = registry.get<ObjectType>(b_entity);
        if (type == ObjectType::PLAYER_SHIP) {
            // Create explosion
            Rectangle rect = registry.get<Rectangle>(a_entity);
            registry.emplace_or_replace<CleanUpComponent::Component>(a_entity);
            rect.width *= 6.f;
            rect.height *= 6.f;
            SmallExplosionEntity::create(registry, rect);
        } else {
            return;
        }
    }    
}

entt::entity BulletEntity::create(entt::registry &object_registry, const Rectangle rect)
{
    const Vector2 resolution = Graphics::getCurrentResolution();
    const Vector2 velocity = {0.f, resolution.y / 2.f};
    const auto projectile_entity = object_registry.create();

    auto &sprite = object_registry.emplace<GraphicsComponent::Animation>(projectile_entity); 
    auto &collider = object_registry.emplace<CollisionComponent::Component>(projectile_entity);
    auto &container = object_registry.emplace<HitboxComponent::Container>(projectile_entity, projectile_entity);
    
    object_registry.emplace<Rectangle>(projectile_entity, rect);
    object_registry.emplace<DamageComponent>(projectile_entity, 10); 
    object_registry.emplace<VelocityComponent>(projectile_entity, velocity);

    object_registry.emplace<GraphicsComponent::RenderPriority>(projectile_entity, GraphicsComponent::RenderPriority::HIGH);
    object_registry.emplace<GraphicsComponent::RenderType>(projectile_entity, GraphicsComponent::RenderType::ANIMATION);
    object_registry.emplace<ObjectType>(projectile_entity, ObjectType::ENEMY_PROJECTILE);

    sprite = GraphicsComponent::createAnimation("bullet_projectile", "fly", rect.width, rect.height); 
    collider = CollisionComponent::create(true, CollisionComponent::Type::OUT_OF_BOUNDS, collisionCallback);
    HitboxComponent::createHitboxInContainerFromSprite(object_registry, container, sprite, "hitbox_1", {rect.x, rect.y});
    return projectile_entity;
}