#pragma once
#include "../include/entt.hpp"

namespace TimerComponent {
    struct Component {
        unsigned int id = 0;;
        float lifetime = 0.f;
        float saved_lifetime;
    };

    struct Container {
        std::vector<Component> timers;
    };

    inline void createTimerInContainer(Container &container, const float lifetime, const unsigned int id)
    {
        Component timer = { .id = id, .lifetime = lifetime, .saved_lifetime = lifetime };
        container.timers.push_back(timer);
    }

    inline bool isDone(const Container &container, const unsigned int timer_id)
    {
        for (const auto &timer : container.timers) {
            if (timer.id == timer_id)
                return timer.lifetime <= 0.f;
        }
        return false;
    }
    
    inline bool isDone(const Component &component) 
    {
        return component.lifetime <= 0.f;
    };

    inline void reset(Container &container, const unsigned int timer_id)
    {
        for (auto &timer : container.timers) {
            if (timer.id == timer_id)
                timer.lifetime = timer.saved_lifetime;
        } 
    }

    inline void reset(Component &component)
    {
        component.lifetime = component.saved_lifetime;
    }
}
