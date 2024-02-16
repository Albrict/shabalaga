#include "fuel_bar.hpp"
#include "graphics_component.hpp"
#include "message_system.hpp"
#include "object_component.hpp"
#include "timer_component.hpp"

namespace FuelBar {
    void proccessMessagesCallback(entt::registry &registry, const entt::entity entity, MessageSystem::Message msg)
    {
        auto &fuel_sprite = registry.get<GraphicsComponent::Sprite>(entity);
        if (fuel_sprite.current_frame <= 0 || fuel_sprite.current_frame >= 5)
            return;
        else {
            const auto type = entt::any_cast<MessageSystem::HudMessage>(msg.msg);
            if (type == MessageSystem::HudMessage::INCREASE_FUEL) {
                const int fuel_decrease_timer = 1;
                auto &container = registry.get<TimerComponent::Container>(entity);
                TimerComponent::reset(container, fuel_decrease_timer);
                --fuel_sprite.current_frame;
            }
        }
    }
}

void FuelBar::create(entt::registry &registry, const Rectangle rect)
{
    const entt::entity fuel_bar = registry.create();
    const std::string_view key = "fuel_bar";
    const float decrease_fuel_time = 10.f;
    const int timer_id = 1;
    TimerComponent::Container &container = registry.emplace<TimerComponent::Container>(fuel_bar);

    registry.emplace<Rectangle>(fuel_bar, rect);
    registry.emplace<ObjectType>(fuel_bar, ObjectType::HUD);

    GraphicsComponent::addSpriteComponent(registry, fuel_bar, key, rect, GraphicsComponent::RenderPriority::NONE);
    TimerComponent::createTimerInContainer(container, decrease_fuel_time, timer_id);
    MessageSystem::registrEntity(registry, fuel_bar, MessageSystem::Type::HUD_MESSAGE, proccessMessagesCallback);
}
