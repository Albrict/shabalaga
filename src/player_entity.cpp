#include "player_entity.hpp"
#include "clean_up_component.hpp"
#include "collision_component.hpp"
#include "engine_entity.hpp"
#include "explosion_entity.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "input_component.hpp"
#include "message_system.hpp"
#include "object_component.hpp"
#include "ship_components.hpp"
#include "weapon_entity.hpp"

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
        if (type == ObjectType::PROJECTILE || type == ObjectType::PICK_UP)
            return;
        if (type == ObjectType::ENEMY_PROJECTILE || type == ObjectType::EXPLOSION)
            damage = registry.get<DamageComponent>(b_entity).damage;
        auto &sprite = registry.get<GraphicsComponent::Sprite>(a_entity);
        auto &health = registry.get<HealthComponent>(a_entity);
        health.health -= damage;
        if (health.health <= 0) 
            destroy(registry, a_entity);
        else if (sprite.current_frame < 3)
            ++sprite.current_frame;
    }
    
    void fuelOut(entt::registry &object_registry, const entt::entity player)
    {
        const entt::entity engine = object_registry.get<ShipComponents::Container>(player).engine; 
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float velocity_y = resolution.y / 3.f;
        const float velocity_x = GetRandomValue(-resolution.x / 2.f, resolution.x / 2.f);
        auto &engine_animation = object_registry.get<GraphicsComponent::Animation>(engine);
        
        engine_animation.tag = ResourceSystem::getAsepriteTag("base_engine", 0); 
        
        object_registry.emplace<VelocityComponent>(player, (VelocityComponent){velocity_x, velocity_y});
        object_registry.remove<InputComponent::Container>(player);

        Collision::addDynamicCollisionFromAseprite(object_registry, player, "ship", true, Collision::Type::BOUNDS, collisionCallback);
    }

    void proccessMessagesCallback(entt::registry &registry, const entt::entity entity, MessageSystem::Message msg)
    {
        const auto type = entt::any_cast<MessageSystem::PlayerMessage>(msg.msg);
        switch(type) {
        using MessageSystem::PlayerMessage;
        case PlayerMessage::OUT_OF_FUEL:
            fuelOut(registry, entity);
            break;
        }
    }
}

entt::entity PlayerEntity::create(entt::registry &object_registry, const Rectangle rect)
{
    const auto player_entity = object_registry.create();
    const auto engine = EngineEntity::create(object_registry, EngineEntity::PlayerType::BASIC, rect);
    const auto weapon = WeaponEntity::create(object_registry, rect, WeaponEntity::PlayerType::BIG_SPACE_GUN); 
    const std::string_view key = "ship";

    auto &input_container = object_registry.emplace<InputComponent::Container>(player_entity);
    GraphicsComponent::addSpriteComponent(object_registry, player_entity, key, rect, 
                                          GraphicsComponent::RenderPriority::MIDDLE);

    MessageSystem::registrEntity(object_registry, player_entity, MessageSystem::Type::PLAYER_MESSAGE, proccessMessagesCallback);
    ShipComponents::addShipComponents(object_registry, player_entity, rect, ObjectType::PLAYER_SHIP, 100);     
    ShipComponents::attachComponents(object_registry, player_entity, weapon, engine);

    InputComponent::create(input_container, moveLeft, KEY_A, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveRight, KEY_D, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveUp, KEY_W, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveDown, KEY_S, InputComponent::Type::DOWN);

    Collision::addDynamicCollisionFromAseprite(object_registry, player_entity, key, true, Collision::Type::BOUNDS, collisionCallback);
    return player_entity;
}

void PlayerEntity::destroy(entt::registry &registry, const entt::entity player)
{
    const auto ship_components = registry.try_get<ShipComponents::Container>(player);
    if (ship_components) {
        const auto &sprite = registry.get<GraphicsComponent::Sprite>(player); 
        const auto &rectangle = registry.get<Rectangle>(player); 

        registry.emplace_or_replace<CleanUpComponent::Component>(ship_components->engine); 
        registry.emplace_or_replace<CleanUpComponent::Component>(ship_components->weapon); 
        registry.emplace_or_replace<CleanUpComponent::Component>(player); 
        
        ExplosionEntity::create(registry, rectangle, ExplosionEntity::Type::PLAYER_EXPLOSION);
        PlaySound(ResourceSystem::getSound("enemy_destroyed_01"));
        MessageSystem::unregistrEntity(registry, player, MessageSystem::Type::PLAYER_MESSAGE);
    }
}
