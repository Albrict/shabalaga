#include "auto_cannon_projectile_entity.hpp"
#include "clean_up_component.hpp"
#include "collison_component.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
#include "object_component.hpp"
#include "small_explosion_entity.hpp"

namespace AutoCannonProjectileEntity {

    void playerProjectileCollision(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity)
    {
        const auto type = registry.get<ObjectType>(b_entity);
        if (type == ObjectType::ENEMY_SHIP) {
            const Rectangle rect = registry.get<Rectangle>(a_entity);
            const int sound = GetRandomValue(0, 1);
            registry.emplace_or_replace<CleanUpComponent::Component>(a_entity);
            SmallExplosionEntity::create(registry, rect);
            if (sound > 0)
                PlaySound(ResourceSystem::getSound("hitsound_01"));
            else
                PlaySound(ResourceSystem::getSound("hitsound_02"));
        } else {
            return;
        }
    }
}

entt::entity AutoCannonProjectileEntity::create(entt::registry &object_registry, const Rectangle rect)
{
    const entt::entity projectile_entity = object_registry.create();
    const Vector2 resolution = Graphics::getCurrentResolution();
    const Vector2 velocity = {0.f, resolution.y / -1.5f};
    const std::string_view key = "auto_cannon_projectile";
    auto &collider = object_registry.emplace<CollisionComponent::Component>(projectile_entity);
    auto &container = object_registry.emplace<HitboxComponent::Container>(projectile_entity, projectile_entity);
    
    object_registry.emplace<Rectangle>(projectile_entity, rect);
    object_registry.emplace<DamageComponent>(projectile_entity, 100);
    object_registry.emplace<VelocityComponent>(projectile_entity, velocity);
    object_registry.emplace<ObjectType>(projectile_entity, ObjectType::PROJECTILE);

    GraphicsComponent::addAnimationComponent(object_registry, projectile_entity, key, 0, rect, GraphicsComponent::RenderPriority::HIGH);
    collider = CollisionComponent::create(true, CollisionComponent::Type::OUT_OF_BOUNDS, playerProjectileCollision);

    HitboxComponent::loadHitboxesInContainer(container, "auto_cannon_projectile", rect);
    return projectile_entity;
}
