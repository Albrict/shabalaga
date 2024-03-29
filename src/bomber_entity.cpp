#include "bomber_entity.hpp"
#include "behavior_component.hpp"
#include "clean_up_component.hpp"
#include "collision_component.hpp"
#include "engine_entity.hpp"
#include "explosion_entity.hpp"
#include "graphics_component.hpp"
#include "message_system.hpp"
#include "object_component.hpp"
#include "ship_components.hpp"

namespace BomberEntity {
    void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity)
    {
        const auto &rect = registry.get<Rectangle>(a_entity);
        if (rect.y + rect.height <= 0.f)
            return;
        const auto type = registry.get<ObjectType>(b_entity);
        if (type == ObjectType::PLAYER_SHIP|| type == ObjectType::PROJECTILE) {
            auto &health = registry.get<HealthComponent>(a_entity); 
            if (type == ObjectType::PLAYER_SHIP) {
                health.health -= health.health;
            } else {
                const int damage = registry.get<DamageComponent>(b_entity).damage;
                health.health -= damage;
            }
            if (health.health <= 0)
                destroy(registry, a_entity);
        } 
    }
}

void BomberEntity::create(entt::registry &object_registry, const Rectangle rect, const Rectangle player_position)
{
    const auto bomber = object_registry.create();
    const auto weapon = object_registry.create();
    const auto engine = EngineEntity::create(object_registry, EngineEntity::EnemyType::BOMBER, rect);
    const std::string_view key = "bomber";
    const float time_to_travel = 0.8f;
    Vector2 velocity =  {0.f, 0.f};
    
    const float distance_y = player_position.y - rect.y;
    velocity.y = player_position.y / time_to_travel;
    if (player_position.x > rect.x)  {
        const float distance = player_position.x - rect.x;
        const float average_velocity = distance / time_to_travel;
        velocity.x = average_velocity;
    } else {
        const float distance = rect.x - player_position.x;
        const float average_velocity = distance / time_to_travel;
        velocity.x = -average_velocity;
    }

    object_registry.emplace<Rectangle>(weapon);
    object_registry.emplace<VelocityComponent>(bomber, velocity);
    object_registry.emplace<ObjectComponent::Score>(bomber, 200); 
    object_registry.emplace<BehaviorComponent::Type>(bomber, BehaviorComponent::Type::BOMBER);
    object_registry.emplace<ObjectComponent::PlayerPosition>(bomber, player_position); 
    ShipComponents::addShipComponents(object_registry, bomber, rect, ObjectType::ENEMY_SHIP, 50);     
    ShipComponents::attachComponents(object_registry, bomber, weapon, engine);
    GraphicsComponent::addSpriteComponent(object_registry, bomber, key, rect, 
                                          GraphicsComponent::RenderPriority::MIDDLE);
    Collision::addDynamicCollisionFromAseprite(object_registry, bomber, key, true, Collision::Type::OUT_OF_BOUNDS, collisionCallback);
}

void BomberEntity::destroy(entt::registry &registry, const entt::entity entity)
{
    const auto ship_components = registry.try_get<ShipComponents::Container>(entity);
    const int sound = GetRandomValue(0, 1);
    if (ship_components) {
        const auto &sprite = registry.get<GraphicsComponent::Sprite>(entity); 
        const int score = registry.get<ObjectComponent::Score>(entity).score; 
        const Rectangle rect = registry.get<Rectangle>(entity);
        registry.emplace_or_replace<CleanUpComponent::Component>(ship_components->engine); 
        registry.emplace_or_replace<CleanUpComponent::Component>(entity); 
         
        ExplosionEntity::create(registry, rect, ExplosionEntity::Type::BOMBER_EXPLOSION);
        if (sound > 0)
            PlaySound(ResourceSystem::getSound("enemy_destroyed_01"));
        else
            PlaySound(ResourceSystem::getSound("enemy_destroyed_02"));
        MessageSystem::Message message = {
           .msg = score,
           .type = MessageSystem::Type::GAME_MASTER_MESSAGE
        };
    }
}
