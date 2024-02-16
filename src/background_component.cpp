#include "background_component.hpp"
#include "graphics_component.hpp"

entt::entity BackgroundComponent::create(entt::registry &object_registry, const Texture2D texture, const float scroll_speed_y, const float scroll_speed_x)
{
    const auto background_entity = object_registry.create();
    auto &background = object_registry.emplace<Background>(background_entity);
    background = {
        .background = texture,
        .width = static_cast<float>(texture.width),
        .height = static_cast<float>(texture.height),
        .scroll_speed_x = scroll_speed_x,
        .scroll_speed_y = scroll_speed_y,
        .offset = {0.f, 0.f},
    };
    object_registry.emplace<GraphicsComponent::RenderType>(background_entity, GraphicsComponent::RenderType::BACKGROUND);    
    return background_entity;
}
