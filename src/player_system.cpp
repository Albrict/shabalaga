#include "player_system.hpp"
#include "player_component.hpp"
#include "ship_components.hpp"
#include "../include/raymath.h"

void PlayerSystem::proccessEvents(entt::registry &registry)
{
    auto view = registry.view<ShipComponent>();
    for (auto [entity, ship] : view.each()) {
        if (IsKeyDown(KEY_A))
            ship.velocity.x -= 10.f * GetFrameTime();
        if (IsKeyDown(KEY_D))
            ship.velocity.x += 10.f * GetFrameTime();
    }
}

void PlayerSystem::update(entt::registry &registry)
{
}

void PlayerSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<PlayerComponent, ShipComponent>();
    for (auto [entity, player, ship] : view.each())
        player.sprite->draw(0, ship.position.x, ship.position.y, WHITE); 
}
