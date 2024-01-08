#include "engine_entity.hpp"
#include "graphics_component.hpp"
#include "object_component.hpp"

entt::entity EngineEntity::create(entt::registry &object_registry, const Type type, const Rectangle rect)
{
    const auto engine_entity = object_registry.create();
    auto &sprite = object_registry.emplace<GraphicsComponent::Animation>(engine_entity);
    object_registry.emplace<Rectangle>(engine_entity, rect);

    object_registry.emplace<GraphicsComponent::RenderPriority>(engine_entity, GraphicsComponent::RenderPriority::HIGH);
    object_registry.emplace<GraphicsComponent::RenderType>(engine_entity, GraphicsComponent::RenderType::ANIMATION);
    object_registry.emplace<ObjectType>(engine_entity, ObjectType::SHIP_COMPONENT);
    
    switch(type) {
    case Type::BASIC:
        sprite = GraphicsComponent::createAnimation("base_engine", 0, rect.width, rect.height);
        break;
    case Type::FIGHTER:
        sprite = GraphicsComponent::createAnimation("fighter_engine", 0, rect.width, rect.height);
        break;
    }
    return engine_entity;
}
