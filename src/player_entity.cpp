#include "player_entity.hpp"
#include "clean_up_component.hpp"
#include "collison_component.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
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

entt::entity PlayerEntity::create(entt::registry &object_registry, const Vector2 position, const float width, const float height)
{
    const Rectangle entity_rectangle = { position.x, position.y, width, height };
    const std::string_view player_sprite_key = "ship";
    auto player_entity = object_registry.create();

    auto &player_sprite = object_registry.emplace<GraphicsComponent::Sprite>(player_entity);
    auto &hitbox_container = object_registry.emplace<HitboxComponent::Container>(player_entity, player_entity);
    auto &input_container = object_registry.emplace<InputComponent::Container>(player_entity);
    auto &collider = object_registry.emplace<CollisionComponent::Component>(player_entity); 
    
    object_registry.emplace<Rectangle>(player_entity, entity_rectangle);
    object_registry.emplace<HealthComponent>(player_entity, 100);
    object_registry.emplace<ShipComponents::Container>(player_entity);
    
    object_registry.emplace<GraphicsComponent::RenderPriority>(player_entity, GraphicsComponent::RenderPriority::MIDDLE);
    object_registry.emplace<GraphicsComponent::RenderType>(player_entity, GraphicsComponent::RenderType::SPRITE);
    object_registry.emplace<ObjectType>(player_entity, ObjectType::PLAYER_SHIP);

    player_sprite = GraphicsComponent::createSprite(player_sprite_key, width, height);
    
    InputComponent::create(input_container, moveLeft, KEY_A, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveRight, KEY_D, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveUp, KEY_W, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveDown, KEY_S, InputComponent::Type::DOWN);

    HitboxComponent::loadHitboxesInContainer(hitbox_container, player_sprite_key, entity_rectangle);

    collider = CollisionComponent::create(true, CollisionComponent::Type::BOUNDS, collisionCallback, nullptr); 
     
    return player_entity;
}
