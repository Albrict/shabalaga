#include "player_system.hpp"
#include "graphics.hpp"
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
    auto view = registry.view<PlayerComponent, ShipComponent>();
    for (auto [entity, player, ship] : view.each())
        player.engine_sprite->updateTag(); 
}

void PlayerSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<PlayerComponent, ShipComponent>();
    for (auto [entity, player, ship] : view.each()) {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float width = resolution.x / 10.f;
        const float height = resolution.y / 10.f;

        const Rectangle dest = {ship.position.x, ship.position.y, width, height};
        player.engine_sprite->drawTagPro(dest, {0.f, 0.f}, 0.f, WHITE);
        player.ship_sprite->drawPro(0, dest, {0.f, 0.f}, 0.f, WHITE); 
    }
}
