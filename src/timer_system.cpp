#include "timer_system.hpp"
#include "timer_component.hpp"
#include "../include/raylib.h"

void TimerSystem::update(entt::registry &registry)
{
    auto view = registry.view<TimerComponent::Container>();
    for (auto [entity, container] : view.each()) {
        for (auto &timer : container.timers) {
            if (timer.lifetime > 0.f)
                timer.lifetime -= GetFrameTime();
        }
    }
}
