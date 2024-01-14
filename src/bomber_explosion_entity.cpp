#include "bomber_explosion_entity.hpp"
#include "clean_up_component.hpp"
#include "collison_component.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
#include "object_component.hpp"

namespace BomberExplosionEntity {
    void animationCallback(entt::registry &registry, const entt::entity entity)
    {
        registry.emplace<CleanUpComponent::Component>(entity);
    }

    void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity) 
    {
        const int damage = registry.get<DamageComponent>(a_entity).damage;
        auto &health = registry.get<HealthComponent>(b_entity);
        health.health -= damage; 
    }
}

void BomberExplosionEntity::create(entt::registry &registry, const Rectangle rect)
{
    const auto explosion = registry.create();
    const int explosion_tag_id = 0;
    const int last_frame = 8;
    const std::string_view sprite_key = "bomber_explosion";
    auto &sprite = registry.emplace<GraphicsComponent::Animation>(explosion);
    auto &collider = registry.emplace<CollisionComponent::Component>(explosion, 
                         CollisionComponent::create(true, CollisionComponent::Type::NONE, nullptr, nullptr));
    auto &container = registry.emplace<HitboxComponent::Container>(explosion, explosion);
   
    registry.emplace<DamageComponent>(explosion, 100);
    registry.emplace<GraphicsComponent::RenderPriority>(explosion, GraphicsComponent::RenderPriority::LOW);
    registry.emplace<GraphicsComponent::RenderType>(explosion, GraphicsComponent::RenderType::EFFECT);
    registry.emplace<Rectangle>(explosion, rect);

    registry.emplace<ObjectType>(explosion, ObjectType::EXPLOSION);

    sprite = GraphicsComponent::createAnimation(sprite_key, 0, rect.width, rect.height);
    GraphicsComponent::addCallback(sprite, explosion_tag_id, last_frame, animationCallback);
    HitboxComponent::loadHitboxesInContainer(container, sprite_key, rect);
}
