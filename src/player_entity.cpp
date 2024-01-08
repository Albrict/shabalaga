#include "player_entity.hpp"
#include "auto_cannon_entity.hpp"
#include "clean_up_component.hpp"
#include "collison_component.hpp"
#include "engine_entity.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "input_component.hpp"
#include "object_component.hpp"
#include "player_explosion_entity.hpp"
#include "ship_components.hpp"

namespace PlayerEntity {
    void moveLeft(entt::registry &registry, const entt::entity entity)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float movement = resolution.x / 3.f * GetFrameTime();
        auto &position = registry.get<Rectangle>(entity);
        position.x -= movement; 
    }

    void moveRight(entt::registry &registry, const entt::entity entity)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float movement = resolution.x / 3.f * GetFrameTime();
        auto &position = registry.get<Rectangle>(entity);
        position.x += movement; 
    }

    void moveUp(entt::registry &registry, const entt::entity entity)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float movement = resolution.x / 3.f * GetFrameTime();
        auto &position = registry.get<Rectangle>(entity);
        position.y -= movement; 
    }

    void moveDown(entt::registry &registry, const entt::entity entity)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float movement = resolution.x / 3.f * GetFrameTime();
        auto &position = registry.get<Rectangle>(entity);
        position.y += movement; 
    }

    void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity)
    {
        const auto type = registry.get<ObjectType>(b_entity);
        int damage = 0;
        if (type == ObjectType::PROJECTILE)
            return;
        if (type == ObjectType::ENEMY_PROJECTILE)
            damage = registry.get<DamageComponent>(b_entity).damage;
        auto &sprite = registry.get<GraphicsComponent::Sprite>(a_entity);
        auto &health = registry.get<HealthComponent>(a_entity);
        health.health -= damage;
        if (health.health <= 0) {
            const auto ship_components = registry.try_get<ShipComponents::Container>(a_entity);
            if (ship_components) {
                const auto &sprite = registry.get<GraphicsComponent::Sprite>(a_entity); 
                const auto &rectangle = registry.get<Rectangle>(a_entity); 

                registry.emplace<CleanUpComponent::Component>(ship_components->engine); 
                registry.emplace<CleanUpComponent::Component>(ship_components->weapon); 

                registry.remove<ShipComponents::Container>(a_entity);
                registry.remove<CollisionComponent::Component>(a_entity);
                registry.emplace<CleanUpComponent::Component>(a_entity); 
                
                PlayerExplosionEntity::create(registry, rectangle);
                PlaySound(ResourceSystem::getSound("enemy_destroyed_01"));
            }
        }
        if (sprite.current_frame < 3)
            ++sprite.current_frame;
    }
}

entt::entity PlayerEntity::create(entt::registry &object_registry, const Rectangle rect)
{
    const auto player_entity = object_registry.create();
    const auto engine = EngineEntity::create(object_registry, EngineEntity::Type::BASIC, rect);
    const auto weapon = AutoCannonEntity::create(object_registry, rect);
    const std::string_view player_sprite_key = "ship";

    auto &input_container = object_registry.emplace<InputComponent::Container>(player_entity);
    auto &collider = object_registry.emplace<CollisionComponent::Component>(player_entity, 
                         CollisionComponent::create(true, CollisionComponent::Type::BOUNDS, collisionCallback, nullptr));
    
    ShipComponents::addShipComponents(object_registry, player_entity, player_sprite_key, rect, ObjectType::PLAYER_SHIP, 100);     
    ShipComponents::attachComponents(object_registry, player_entity, weapon, engine);
    GraphicsComponent::addSpriteComponent(object_registry, player_entity, player_sprite_key, rect, 
                                          GraphicsComponent::RenderPriority::MIDDLE);

    InputComponent::create(input_container, moveLeft, KEY_A, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveRight, KEY_D, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveUp, KEY_W, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveDown, KEY_S, InputComponent::Type::DOWN);
    return player_entity;
}
