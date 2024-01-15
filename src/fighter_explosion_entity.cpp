#include "fighter_explosion_entity.hpp"
#include "clean_up_component.hpp"
#include "graphics_component.hpp"

namespace FighterExplosionEntity {
    void animationCallback(entt::registry &registry, const entt::entity entity)
    {
        registry.emplace<CleanUpComponent::Component>(entity);
    }
}

void FighterExplosionEntity::create(entt::registry &registry, const Rectangle rect)
{
    const auto explosion = registry.create();
    const int explosion_tag_id = 0;
    const int last_frame = 8;
    const std::string_view key = "fighter_explosion";

    registry.emplace<Rectangle>(explosion, rect);
    GraphicsComponent::addAnimationComponent(registry, explosion, key, 0, rect, GraphicsComponent::RenderPriority::LOW);
    GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, animationCallback);
}
