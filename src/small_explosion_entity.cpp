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
    const entt::entity explosion = registry.create();
    const int explosion_tag_id = 0;
    const int last_frame = 2;
    const std::string_view key = "small_explosion"; 

    registry.emplace<Rectangle>(explosion, rect);
    GraphicsComponent::addAnimationComponent(registry, explosion, key, 0, rect, GraphicsComponent::RenderPriority::LOW);
    GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, animationCallback);
    return explosion;
}
