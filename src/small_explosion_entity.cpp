#include "small_explosion_entity.hpp"
#include "graphics_component.hpp"
#include "clean_up_component.hpp"

namespace SmallExplosionEntity {
    void animationCallback(entt::registry &registry, const entt::entity entity)
    {
        registry.emplace<CleanUpComponent::Component>(entity);
    }
}

entt::entity SmallExplosionEntity::create(entt::registry &registry, const Rectangle rect)
{
    const auto explosion = registry.create();
    const int explosion_tag_id = 0;
    const int last_frame = 2;

    auto &sprite = registry.emplace<GraphicsComponent::Animation>(explosion);
   
    registry.emplace<GraphicsComponent::RenderPriority>(explosion, GraphicsComponent::RenderPriority::HIGH);
    registry.emplace<GraphicsComponent::RenderType>(explosion, GraphicsComponent::RenderType::EFFECT);
    registry.emplace<Rectangle>(explosion, rect);
    sprite = GraphicsComponent::createAnimation("small_explosion", "explosion", rect.width, rect.height);
    GraphicsComponent::addCallback(sprite, explosion_tag_id, last_frame, animationCallback);
    return explosion;
}
