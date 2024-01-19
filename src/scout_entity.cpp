#include "scout_entity.hpp"
#include "behavior_component.hpp"
#include "clean_up_component.hpp"
#include "collison_component.hpp"
#include "engine_entity.hpp"
#include "fuel_pickup_entity.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "message_system.hpp"
#include "object_component.hpp"
#include "scout_explosion.hpp"
#include "ship_components.hpp"
#include "scout_weapon.hpp"
#include "timer_component.hpp"

namespace ScoutEntity {
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
                    
                    ScoutExplosion::create(registry, rect);
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

void ScoutEntity::create(entt::registry &registry, const Rectangle rect)
{
    const auto scout = registry.create();
    const auto weapon = ScoutWeapon::create(registry, rect);
    const auto engine = EngineEntity::create(registry, EngineEntity::Type::SCOUT, rect);
    const Vector2 resolution = Graphics::getCurrentResolution();
    const Vector2 velocity = {0.f, resolution.y / 3.f};
    const std::string_view sprite_key = "scout";
    const int direction_timer = 1; 
    const int collision_timer_id = 2;
    const float timer_lifetime = 2.f;
    const float collision_timer_lifetime = ((rect.y * -1.f) + rect.height) / velocity.y;

    registry.emplace<CollisionComponent::Component>(scout, 
                                             CollisionComponent::create(true, CollisionComponent::Type::OUT_OF_BOUNDS, collisionCallback, nullptr));
    registry.emplace<VelocityComponent>(scout, velocity);
    registry.emplace<ObjectComponent::Score>(scout, 150); 
    registry.emplace<BehaviorComponent::Type>(scout, BehaviorComponent::Type::SCOUT);

    ShipComponents::addShipComponents(registry, scout, sprite_key, rect, ObjectType::ENEMY_SHIP, 50);     
    ShipComponents::attachComponents(registry, scout, weapon, engine);
    GraphicsComponent::addSpriteComponent(registry, scout, sprite_key, rect, 
                                          GraphicsComponent::RenderPriority::MIDDLE);
    TimerComponent::createTimer(registry, scout, timer_lifetime, direction_timer);
    TimerComponent::createTimer(registry, scout, collision_timer_lifetime, collision_timer_id);
}
