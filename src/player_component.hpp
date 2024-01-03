#pragma once
#include "../include/entt.hpp"

namespace PlayerComponent {
    struct Container {
        entt::entity engine = entt::null;
        entt::entity weapon = entt::null;
    };

    inline void attachWeapon(entt::registry &object_registry, const entt::entity player, const entt::entity weapon)
    {
        auto &container = object_registry.get<Container>(player);
        container.weapon = weapon;
    }

    inline void attachEngine(entt::registry &object_registry, const entt::entity player, const entt::entity engine)
    {
        auto &container = object_registry.get<Container>(player);
        container.engine = engine;
    }
}
