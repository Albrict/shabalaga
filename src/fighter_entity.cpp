#include "fighter_entity.hpp"
#include "behavior_component.hpp"
#include "clean_up_component.hpp"
#include "collison_component.hpp"
#include "engine_entity.hpp"
#include "fighter_weapon_entity.hpp"
#include "fighter_explosion_entity.hpp"
#include "graphics_component.hpp"
#include "message_system.hpp"
#include "object_component.hpp"
#include "ship_components.hpp"
#include "timer_component.hpp"
#include "fuel_pickup_entity.hpp"

namespace FighterEntity {
    void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity)
    {
        auto &rect = registry.get<Rectangle>(a_entity);
        if (rect.y + rect.height <= 0.f)
            return;
        const auto type = registry.get<ObjectType>(b_entity);
        if (type == ObjectType::PROJECTILE || type == ObjectType::EXPLOSION) {
            const int damage = registry.get<DamageComponent>(b_entity).damage;
            auto &health = registry.get<HealthComponent>(a_entity); 
            health.health -= damage;
            if (health.health <= 0) {
                const auto ship_components = registry.try_get<ShipComponents::Container>(a_entity);
                const int sound = GetRandomValue(0, 1);
                const int pick_up_chance_spawn = GetRandomValue(0, 10);

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
                    MessageSystem::Message message = {
                       .msg = score,
                       .type = MessageSystem::Type::GAME_MASTER_MESSAGE
                    };
                    MessageSystem::sendMessageToEntity(registry, message);

                    if (pick_up_chance_spawn >= 8) {
                        rect.width /= 3.f;
                        rect.height /= 3.f;
                        FuelPickUpEntity::create(registry, rect);
                    }
                }
            }
        } 
    }
}

entt::entity FighterEntity::create(entt::registry &object_registry, const Rectangle rect, const float fire_cooldown)
{
    const auto fighter_entity = object_registry.create();
    const auto fighter_weapon = FighterWeaponEntity::create(object_registry, rect, fire_cooldown);
    const auto fighter_engine = EngineEntity::create(object_registry, EngineEntity::Type::FIGHTER, rect);
    const int explosion_tag = 1;
    const int last_frame = 7;
    const Vector2 velocity = {0.f, 400.f};
    const int random_direction_timer = 1;
    const std::string_view fighter_sprite_key = "fighter";

    auto &timer_container = object_registry.emplace<TimerComponent::Container>(fighter_entity); 

    object_registry.emplace<CollisionComponent::Component>(fighter_entity, 
                                             CollisionComponent::create(true, CollisionComponent::Type::OUT_OF_BOUNDS, collisionCallback, nullptr));
    object_registry.emplace<VelocityComponent>(fighter_entity, velocity);
    object_registry.emplace<ObjectComponent::Score>(fighter_entity, 50); 
    object_registry.emplace<BehaviorComponent::Type>(fighter_entity, BehaviorComponent::Type::FIGHTER);

    ShipComponents::addShipComponents(object_registry, fighter_entity, fighter_sprite_key, rect, ObjectType::ENEMY_SHIP, 50);     
    ShipComponents::attachComponents(object_registry, fighter_entity, fighter_weapon, fighter_engine);
    GraphicsComponent::addSpriteComponent(object_registry, fighter_entity, fighter_sprite_key, rect, 
                                          GraphicsComponent::RenderPriority::MIDDLE);
    TimerComponent::createTimerInContainer(timer_container, 1.f, random_direction_timer);
    return fighter_entity;
}
