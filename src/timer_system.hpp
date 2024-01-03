#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"
#include "timer_component.hpp"

namespace TimerSystem {
    void update(entt::registry &registry);
    inline void update(TimerComponent::Component &component)
    {
        if (component.lifetime > 0.f)
            component.lifetime -= GetFrameTime();
    }
}
