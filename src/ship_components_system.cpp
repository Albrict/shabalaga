#include "ship_components_system.hpp"
#include "ship_components.hpp"
#include "../include/raylib.h"

void ShipComponentsSystem::update(entt::registry &registry)
{
    auto view = registry.view<ShipComponents::Container>();
    for (auto [entity, container] : view.each()) {
        auto &ship_position = registry.get<Rectangle>(container.ship);
        registry.replace<Rectangle>(container.weapon, ship_position);
        registry.replace<Rectangle>(container.engine, ship_position);
    }
}
