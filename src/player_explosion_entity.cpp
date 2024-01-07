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

    auto &sprite = registry.emplace<GraphicsComponent::Animation>(explosion);
   
    registry.emplace<GraphicsComponent::RenderPriority>(explosion, GraphicsComponent::RenderPriority::HIGH);
    registry.emplace<GraphicsComponent::RenderType>(explosion, GraphicsComponent::RenderType::EFFECT);
    registry.emplace<Rectangle>(explosion, rect);
    sprite = GraphicsComponent::createAnimation("player_explosion", 0, rect.width, rect.height);
    GraphicsComponent::addCallback(sprite, explosion_tag_id, last_frame, animationCallback);
}
