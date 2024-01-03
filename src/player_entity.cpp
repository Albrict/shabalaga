#include "player_entity.hpp"
#include "collison_component.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
#include "input_component.hpp"
#include "object_component.hpp"
#include "ship_components.hpp"

namespace PlayerEntity {
    void moveLeft(entt::registry &registry, const entt::entity entity)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float movement = resolution.x / 4.f * GetFrameTime();
        auto &position = registry.get<Rectangle>(entity);
        position.x -= movement; 
    }

    void moveRight(entt::registry &registry, const entt::entity entity)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float movement = resolution.x / 4.f * GetFrameTime();
        auto &position = registry.get<Rectangle>(entity);
        position.x += movement; 
    }

    void moveUp(entt::registry &registry, const entt::entity entity)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float movement = resolution.x / 4.f * GetFrameTime();
        auto &position = registry.get<Rectangle>(entity);
        position.y -= movement; 
    }

    void moveDown(entt::registry &registry, const entt::entity entity)
    {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float movement = resolution.x / 4.f * GetFrameTime();
        auto &position = registry.get<Rectangle>(entity);
        position.y += movement; 
    }

    void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity)
    {
        auto &sprite = registry.get<GraphicsComponent::Sprite>(a_entity);
        int current_frame = sprite.sprite->getCurrentSpriteFrame();
        if (current_frame < 3) {
            ++current_frame;
            sprite.sprite->setFrame(current_frame);
        }
    }
}

entt::entity PlayerEntity::create(entt::registry &object_registry, const Vector2 position, const std::string_view &sprite,
                                     const float width, const float height)
{
    const Rectangle entity_rectangle = { position.x, position.y, width, height };
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

    player_sprite = GraphicsComponent::createSprite(sprite, width, height);
    
    InputComponent::create(input_container, moveLeft, KEY_A, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveRight, KEY_D, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveUp, KEY_W, InputComponent::Type::DOWN);
    InputComponent::create(input_container, moveDown, KEY_S, InputComponent::Type::DOWN);

    HitboxComponent::createHitboxInContainerFromSprite(object_registry, hitbox_container, player_sprite, "hitbox_1",  position);
    HitboxComponent::createHitboxInContainerFromSprite(object_registry, hitbox_container, player_sprite, "hitbox_2",  position);
    HitboxComponent::createHitboxInContainerFromSprite(object_registry, hitbox_container, player_sprite, "hitbox_3",  position);
    HitboxComponent::createHitboxInContainerFromSprite(object_registry, hitbox_container, player_sprite, "hitbox_4",  position);
    HitboxComponent::createHitboxInContainerFromSprite(object_registry, hitbox_container, player_sprite, "hitbox_5",  position);
    HitboxComponent::createHitboxInContainerFromSprite(object_registry, hitbox_container, player_sprite, "hitbox_6",  position);

    collider = CollisionComponent::create(true, CollisionComponent::Type::BOUNDS, collisionCallback, nullptr); 
     
    return player_entity;
}
