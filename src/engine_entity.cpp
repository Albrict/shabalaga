#include "engine_entity.hpp"
#include "graphics_component.hpp"
#include "object_component.hpp"

entt::entity EngineEntity::create(entt::registry &object_registry, const PlayerType type, const Rectangle rect)
{
    const auto engine_entity = object_registry.create();
    object_registry.emplace<Rectangle>(engine_entity, rect);
    object_registry.emplace<ObjectType>(engine_entity, ObjectType::SHIP_COMPONENT);
    object_registry.emplace<PlayerType>(engine_entity, type);

    switch(type) {
    case BASIC:
        GraphicsComponent::addAnimationComponent(object_registry, engine_entity, "base_engine", 1, rect, GraphicsComponent::RenderPriority::HIGH);
        break;
    case BIG_PULSE_ENGINE:
        GraphicsComponent::addAnimationComponent(object_registry, engine_entity, "big_pulse_engine", 1, rect, GraphicsComponent::RenderPriority::HIGH);
        break;
    case BURST_ENGINE:
        GraphicsComponent::addAnimationComponent(object_registry, engine_entity, "burst_engine", 1, rect, GraphicsComponent::RenderPriority::HIGH);
        break;
    case SUPERCHARGED_ENGINE:
        GraphicsComponent::addAnimationComponent(object_registry, engine_entity, "supercharged_engine", 1, rect, GraphicsComponent::RenderPriority::HIGH);
        break;
    }
    return engine_entity;
}

entt::entity EngineEntity::create(entt::registry &object_registry, const EnemyType type, const Rectangle rect)
{
    const auto engine_entity = object_registry.create();
    object_registry.emplace<Rectangle>(engine_entity, rect);
    object_registry.emplace<ObjectType>(engine_entity, ObjectType::SHIP_COMPONENT);
    switch(type) {
    case FIGHTER:
        GraphicsComponent::addAnimationComponent(object_registry, engine_entity, "fighter_engine", 0, rect, GraphicsComponent::RenderPriority::HIGH);
        break;
    case BOMBER:
        GraphicsComponent::addAnimationComponent(object_registry, engine_entity, "bomber_engine", 0, rect, GraphicsComponent::RenderPriority::HIGH);
        break;
    case SCOUT:
        GraphicsComponent::addAnimationComponent(object_registry, engine_entity, "scout_engine", 0, rect, GraphicsComponent::RenderPriority::HIGH);
        break;
    } 
    return engine_entity;
}
