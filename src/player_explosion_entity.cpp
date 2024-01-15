#include "player_explosion_entity.hpp"
#include "clean_up_component.hpp"
#include "graphics_component.hpp"
#include "message_system.hpp"

namespace PlayerExplosionEntity {
    void animationCallback(entt::registry &registry, const entt::entity entity)
    {
        registry.emplace<CleanUpComponent::Component>(entity);
        MessageSystem::Message msg = {.msg = MessageSystem::PlaySceneMessage::PLAYER_DIED, 
        .type = MessageSystem::Type::PLAY_SCENE_MESSAGE};
        MessageSystem::sendMessage(msg);
    }
}

void PlayerExplosionEntity::create(entt::registry &registry, const Rectangle rect)
{
    const auto explosion = registry.create();
    const int explosion_tag_id = 0;
    const int last_frame = 8;
    const std::string_view key = "player_explosion";

    registry.emplace<Rectangle>(explosion, rect);

    GraphicsComponent::addAnimationComponent(registry, explosion, key, explosion_tag_id, rect, GraphicsComponent::RenderPriority::HIGH);
    GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, animationCallback);
}
