#include "bomber_explosion_entity.hpp"
#include "clean_up_component.hpp"
#include "collison_component.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
#include "object_component.hpp"

namespace BomberExplosionEntity {
    void animationCallbackRemoveComponents(entt::registry &registry, const entt::entity entity)
    {
        registry.remove<CollisionComponent::Component>(entity);
        registry.remove<HitboxComponent::Container>(entity);
    }
    
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
    const int last_explosion_frame = 5;
    const std::string_view key = "bomber_explosion";
    auto &collider = registry.emplace<CollisionComponent::Component>(explosion, 
                         CollisionComponent::create(true, CollisionComponent::Type::NONE, nullptr, nullptr));
    auto &container = registry.emplace<HitboxComponent::Container>(explosion, explosion);
   
    registry.emplace<DamageComponent>(explosion, 100);
    registry.emplace<Rectangle>(explosion, rect);

    registry.emplace<ObjectType>(explosion, ObjectType::EXPLOSION);

    GraphicsComponent::addAnimationComponent(registry, explosion, key, 0, rect, GraphicsComponent::RenderPriority::LOW);
    GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, animationCallback);
    GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_explosion_frame, animationCallbackRemoveComponents);
    HitboxComponent::loadHitboxesInContainer(container, key, rect);
}
