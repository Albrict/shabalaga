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
    
    inline void createFinishedTimerInContainer(Container &container, const float lifetime, const unsigned int id)
    {
        Component timer = { .id = id, .lifetime = 0.f, .saved_lifetime = lifetime };
        container.timers.push_back(timer);
    }

    inline void changeTimerCooldown(Container &container, const float new_lifetime, const unsigned int id)
    {
        for (auto &timer : container.timers) {
            if (timer.id == id)
                timer = {.lifetime = new_lifetime, .saved_lifetime = new_lifetime};
        } 
    }
    
    inline float getTimerLifetime(const Container &container, const unsigned int timer_id)
    {
        for (const auto &timer : container.timers) {
            if (timer.id == timer_id)
                return timer.saved_lifetime;
        }
        return 0.f;
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

    inline void endTimer(Container &container, const unsigned int timer_id)
    {
        for (auto &timer : container.timers) {
            if (timer.id == timer_id)
                timer.lifetime = -1.f;
        }
    }

    inline void reset(Component &component)
    {
        component.lifetime = component.saved_lifetime;
    }
}
