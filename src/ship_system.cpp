#include "ship_system.hpp"
#include "player_component.hpp"
#include "ship_components.hpp"
#include "../include/raymath.h"
#include "player_system.hpp"

void ShipSystem::proccessEvents(entt::registry &registry)
{
    auto view = registry.view<ShipType>();
    for (auto [entity, type] : view.each()) {
        switch(type) {
        using enum ShipType;
        case PLAYER:
            PlayerSystem::proccessEvents(registry);
            break;
        }
    }
}

void ShipSystem::update(entt::registry &registry)
{
    auto view = registry.view<ShipType, ShipComponent>();
    for (auto [entity, type, ship] : view.each()) {
        ship.position = Vector2Add(ship.position, ship.velocity);
        switch(type) {
        using enum ShipType;
        case PLAYER:
            PlayerSystem::update(registry);
            break;
        }
    }
}

void ShipSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<ShipType>();
    for (auto [entity, type] : view.each()) {
        switch(type) {
        using enum ShipType;
        case PLAYER:
            PlayerSystem::draw(registry);
            break;
        }
    }
}
