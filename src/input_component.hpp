#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace InputComponent {
    typedef void(*callback)(entt::registry &registry, const entt::entity entity);
    enum class Type {
        DOWN,
        PRESS,
        NONE
    };
    struct Component {
        callback cb = nullptr;
        int key = KEY_NULL;
        Type type = Type::NONE;
    };

    struct Container {
        std::vector<Component> inputs {};
    };

    inline void create(Container &container, callback cb_func, const int key, const Type type)
    {
        Component input {
            .cb = cb_func,
            .key = key,
            .type = type
        };
        container.inputs.push_back(input);
    }
}
