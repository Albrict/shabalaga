#include "fighter_entity.hpp"
#include "behavior_component.hpp"
#include "clean_up_component.hpp"
#include "collison_component.hpp"
#include "engine_entity.hpp"
#include "fighter_weapon_entity.hpp"
#include "fighter_explosion_entity.hpp"
#include "game_master_system.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
#include "object_component.hpp"
#include "ship_components.hpp"
#include "timer_component.hpp"

namespace FighterEntity {
    void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity)
    {
        const auto &rect = registry.get<Rectangle>(a_entity);
        if (rect.y + rect.height <= 0.f)
            return;
        const auto type = registry.get<ObjectType>(b_entity);
        if (type == ObjectType::PROJECTILE) {
            const int damage = registry.get<DamageComponent>(b_entity).damage;
            auto &health = registry.get<HealthComponent>(a_entity); 
            health.health -= damage;
            if (health.health <= 0) {
                const auto ship_components = registry.try_get<ShipComponents::Container>(a_entity);
                const int sound = GetRandomValue(0, 1);
                if (ship_components) {
                    const auto &sprite = registry.get<GraphicsComponent::Sprite>(a_entity); 
                    const int score = registry.get<ObjectComponent::Score>(a_entity).score; 

                    registry.emplace<CleanUpComponent::Component>(ship_components->engine); 
                    registry.emplace<CleanUpComponent::Component>(ship_components->weapon); 

                    registry.remove<BehaviorComponent::Type>(a_entity);  
                    registry.remove<ShipComponents::Container>(a_entity);
                    registry.remove<CollisionComponent::Component>(a_entity);
                    registry.emplace<CleanUpComponent::Component>(a_entity); 
                    
                    FighterExplosionEntity::create(registry, rect);
                    if (sound > 0)
                        PlaySound(ResourceSystem::getSound("enemy_destroyed_01"));
                    else
                        PlaySound(ResourceSystem::getSound("enemy_destroyed_02"));
                    GameMasterSystem::increaseScore(score);
                }
            }
        } 
    }
}

entt::entity FighterEntity::create(entt::registry &object_registry, const Vector2 position, const float width, const float height)
{
    const auto fighter_weapon = FighterWeaponEntity::create(object_registry, {position.x, position.y, width, height});
    const auto fighter_engine = EngineEntity::create(object_registry, EngineEntity::Type::FIGHTER, position, width, height);
    const auto fighter_entity = object_registry.create();
    const int explosion_tag = 1;
    const int last_frame = 7;
    const Vector2 velocity = {0.f, 400.f};
    const int random_direction_timer = 1;
    const std::string_view fighter_sprite_key = "fighter";
    const Rectangle rect = {position.x, position.y, width, height};

    auto &fighter_sprite = object_registry.emplace<GraphicsComponent::Sprite>(fighter_entity);
    auto &hitbox_container = object_registry.emplace<HitboxComponent::Container>(fighter_entity, fighter_entity);
    auto &collider = object_registry.emplace<CollisionComponent::Component>(fighter_entity); 
    auto &timer_container = object_registry.emplace<TimerComponent::Container>(fighter_entity); 

    object_registry.emplace<Rectangle>(fighter_entity, rect);
    object_registry.emplace<HealthComponent>(fighter_entity, 100);
    object_registry.emplace<VelocityComponent>(fighter_entity, velocity);
    object_registry.emplace<ObjectComponent::Score>(fighter_entity, 50); 

    object_registry.emplace<ShipComponents::Container>(fighter_entity);

    object_registry.emplace<GraphicsComponent::RenderPriority>(fighter_entity, GraphicsComponent::RenderPriority::MIDDLE);
    object_registry.emplace<BehaviorComponent::Type>(fighter_entity, BehaviorComponent::Type::FIGHTER);
    object_registry.emplace<ObjectType>(fighter_entity, ObjectType::ENEMY_SHIP);
    object_registry.emplace<GraphicsComponent::RenderType>(fighter_entity, GraphicsComponent::RenderType::SPRITE);
 
    fighter_sprite = GraphicsComponent::createSprite("fighter", width, height);
    collider = CollisionComponent::create(true, CollisionComponent::Type::OUT_OF_BOUNDS, collisionCallback, nullptr); 
    
    HitboxComponent::loadHitboxesInContainer(hitbox_container, fighter_sprite_key, rect);
    
    ShipComponents::attachComponents(object_registry, fighter_entity, fighter_weapon, fighter_engine);

    TimerComponent::createTimerInContainer(timer_container, 1.f, random_direction_timer);
    return fighter_entity;
}
