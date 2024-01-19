#include "engine_entity.hpp"
#include "graphics_component.hpp"
#include "object_component.hpp"

entt::entity EngineEntity::create(entt::registry &object_registry, const Type type, const Rectangle rect)
{
    const auto engine_entity = object_registry.create();
    object_registry.emplace<Rectangle>(engine_entity, rect);
    object_registry.emplace<ObjectType>(engine_entity, ObjectType::SHIP_COMPONENT);

    switch(type) {
    case Type::BASIC:
        GraphicsComponent::addAnimationComponent(object_registry, engine_entity, "base_engine", 1, rect, GraphicsComponent::RenderPriority::HIGH);
        break;
    case Type::FIGHTER:
        GraphicsComponent::addAnimationComponent(object_registry, engine_entity, "fighter_engine", 0, rect, GraphicsComponent::RenderPriority::HIGH);
        break;
    case Type::BOMBER:
        GraphicsComponent::addAnimationComponent(object_registry, engine_entity, "bomber_engine", 0, rect, GraphicsComponent::RenderPriority::HIGH);
        break;
    case Type::SCOUT:
        GraphicsComponent::addAnimationComponent(object_registry, engine_entity, "scout_engine", 0, rect, GraphicsComponent::RenderPriority::HIGH);
        break;
    }
    return engine_entity;
}
