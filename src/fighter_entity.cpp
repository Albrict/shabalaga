#include "fighter_entity.hpp"
#include "behavior_component.hpp"
#include "clean_up_component.hpp"
#include "collision_component.hpp"
#include "engine_entity.hpp"
#include "explosion_entity.hpp"
#include "graphics_component.hpp"
#include "message_system.hpp"
#include "object_component.hpp"
#include "ship_components.hpp"
#include "timer_component.hpp"
#include "fuel_pickup_entity.hpp"
#include "weapon_entity.hpp"

namespace FighterEntity {
    void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity)
    {
        const auto &rect = registry.get<Rectangle>(a_entity);
        if (rect.y + rect.height <= 0.f)
            return;
        const auto type = registry.get<ObjectType>(b_entity);
        if (type == ObjectType::PROJECTILE || type == ObjectType::EXPLOSION) {
            const int damage = registry.get<DamageComponent>(b_entity).damage;
            auto &health = registry.get<HealthComponent>(a_entity); 
            health.health -= damage;
            if (health.health <= 0)
                FighterEntity::destroy(registry, a_entity);
        }
    }
}

entt::entity FighterEntity::create(entt::registry &object_registry, const Rectangle rect, const float fire_cooldown)
{
    const auto fighter_entity = object_registry.create();
    const auto fighter_weapon = WeaponEntity::create(object_registry, rect, WeaponEntity::EnemyType::FIGHTER_WEAPON, fire_cooldown);
    const auto fighter_engine = EngineEntity::create(object_registry, EngineEntity::EnemyType::FIGHTER, rect);
    const int explosion_tag = 1;
    const int last_frame = 7;
    const Vector2 velocity = {0.f, 400.f};
    const int random_direction_timer = 1;
    const std::string_view key = "fighter";

    auto &timer_container = object_registry.emplace<TimerComponent::Container>(fighter_entity); 

    object_registry.emplace<VelocityComponent>(fighter_entity, velocity);
    object_registry.emplace<ObjectComponent::Score>(fighter_entity, 50); 
    object_registry.emplace<BehaviorComponent::Type>(fighter_entity, BehaviorComponent::Type::FIGHTER);

    ShipComponents::addShipComponents(object_registry, fighter_entity, rect, ObjectType::ENEMY_SHIP, 50);     
    ShipComponents::attachComponents(object_registry, fighter_entity, fighter_weapon, fighter_engine);
    GraphicsComponent::addSpriteComponent(object_registry, fighter_entity, key, rect, 
                                          GraphicsComponent::RenderPriority::MIDDLE);
    TimerComponent::createTimerInContainer(timer_container, 1.f, random_direction_timer);
    Collision::addDynamicCollisionFromAseprite(object_registry, fighter_entity, key, true, Collision::Type::OUT_OF_BOUNDS, collisionCallback);
    return fighter_entity;
}

void FighterEntity::destroy(entt::registry &registry, const entt::entity entity)
{
    const auto rect = registry.get<Rectangle>(entity);
    const auto ship_components = registry.try_get<ShipComponents::Container>(entity);
    const int sound = GetRandomValue(0, 1);
    const int pick_up_chance_spawn = GetRandomValue(0, 10);
    
    if (!ship_components)
        return;
    const auto &sprite = registry.get<GraphicsComponent::Sprite>(entity); 
    const int score = registry.get<ObjectComponent::Score>(entity).score; 

    registry.emplace_or_replace<CleanUpComponent::Component>(ship_components->engine); 
    registry.emplace_or_replace<CleanUpComponent::Component>(ship_components->weapon); 
    registry.emplace_or_replace<CleanUpComponent::Component>(entity); 
    
    if (sound > 0)
        PlaySound(ResourceSystem::getSound("enemy_destroyed_01"));
    else
        PlaySound(ResourceSystem::getSound("enemy_destroyed_02"));

    if (pick_up_chance_spawn >= 8) {
        Rectangle pick_up_rect = rect;
        pick_up_rect.width /= 3.f;
        pick_up_rect.height /= 3.f;
        FuelPickUpEntity::create(registry, pick_up_rect);
    }
    ExplosionEntity::create(registry, rect, ExplosionEntity::Type::FIGHTER_EXPLOSION);
    
    MessageSystem::Message message = {
       .msg = score,
       .type = MessageSystem::Type::GAME_MASTER_MESSAGE
    };
    MessageSystem::sendMessageToEntity(registry, message);
}
