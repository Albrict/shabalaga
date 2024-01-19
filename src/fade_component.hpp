#pragma once

namespace FadeEffect {
    enum class Type {
        FADE_IN,
        FADE_OUT
    };

    struct Component {
        float lifetime = 0.f;
        float saved_lifetime = 0.f;
        bool paused = false;
        Type type;
    };

    inline Component create(const float lifetime, const Type type, const bool paused = false)
    {
        return (Component) {
            .lifetime = lifetime,
            .saved_lifetime = lifetime,
            .paused = paused,
            .type = type
        };
    }

    inline bool isDone(const Component component)
    {
        return component.lifetime <= 0.f;
    }

    inline void resume(Component &component)
    {
        component.paused = false;
    }
}
