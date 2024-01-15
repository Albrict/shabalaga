#include "fuel_system.hpp"
#include "graphics_component.hpp"
#include "message_system.hpp"
#include "timer_component.hpp"

namespace FuelSystem {
    int current_fuel_level = 0; 
}

void FuelSystem::proccessEvents(entt::registry &registry, const entt::entity entity)
{
    const int timer_id = 1;
    auto &container = registry.get<TimerComponent::Container>(entity);
    auto &sprite = registry.get<GraphicsComponent::Sprite>(entity);
    if (TimerComponent::isDone(container, timer_id)) {
        if (current_fuel_level < 5) {
            ++current_fuel_level;
        } else {
            MessageSystem::Message msg = {.msg = MessageSystem::PlaySceneMessage::OUT_OF_FUEL, 
            .type = MessageSystem::Type::PLAY_SCENE_MESSAGE};
            MessageSystem::sendMessage(msg);
        }
        TimerComponent::reset(container, timer_id);
    }
    sprite.current_frame = current_fuel_level;
}

void FuelSystem::increaseFuelLevel()
{
    if (current_fuel_level > 0)
        --current_fuel_level;
}

void FuelSystem::reset()
{
    current_fuel_level = 0;
}
