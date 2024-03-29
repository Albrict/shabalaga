#pragma once
#include "../include/entt.hpp"
#include "object_component.hpp"

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

    inline void addShipComponents(entt::registry &registry, const entt::entity entity, const Rectangle rect, const ObjectType object_type, int health)
    {
        registry.emplace<Rectangle>(entity, rect);
        registry.emplace<HealthComponent>(entity, health);
        registry.emplace<ShipComponents::Container>(entity);
        registry.emplace<ObjectType>(entity, object_type);
    }
}
