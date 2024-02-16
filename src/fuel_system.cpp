#include "fuel_system.hpp"
#include "graphics_component.hpp"
#include "message_system.hpp"
#include "timer_component.hpp"

namespace FuelSystem {
    bool message_sended = false;
}

void FuelSystem::proccessEvents(entt::registry &registry, const entt::entity entity)
{
    const int timer_id = 1;
    auto &container = registry.get<TimerComponent::Container>(entity);
    auto &sprite = registry.get<GraphicsComponent::Sprite>(entity);
    if (TimerComponent::isDone(container, timer_id)) {
        if (sprite.current_frame < 5) {
            ++sprite.current_frame;
        } else if (!message_sended){
            MessageSystem::Message msg = {.msg = MessageSystem::PlayerMessage::OUT_OF_FUEL, 
            .type = MessageSystem::Type::PLAYER_MESSAGE};
            MessageSystem::sendMessageToEntity(registry, msg);
            message_sended = true;
        }
        TimerComponent::reset(container, timer_id);
    }
}
