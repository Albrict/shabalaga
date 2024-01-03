#pragma once
#include "../include/entt.hpp"

namespace ShipComponents {
    struct Container {
        entt::entity ship = entt::null;
        entt::entity weapon = entt::null;
        entt::entity engine = entt::null;
    };

    inline void attachComponents(entt::registry &registry, const entt::entity ship, const entt::entity weapon, const entt::entity engine )
    {
        auto &container = registry.get<Container>(ship);
        container = {
            .ship = ship,
            .weapon = weapon,
            .engine = engine
        };
    }
}
