#pragma once

namespace FadeComponent {
    enum class Type {
        FADE_IN,
        FADE_OUT
    };

    struct Component {
        const float fade_value;
        const Type type;
        float alpha = 0.f;
    };

    inline Component create(const float fade_value, const Type type)
    {
        return (Component) {
            .fade_value = fade_value,
            .type = type,
        };
    }
}
