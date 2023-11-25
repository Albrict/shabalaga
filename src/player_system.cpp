#include "player_system.hpp"
#include "graphics.hpp"
#include "player_component.hpp"
#include "resource_system.hpp"
#include "ship_components.hpp"
#include "../include/raymath.h"
#include "weapon_component.hpp"
#include "projectile_component.hpp"
#include "weapon_system.hpp"

namespace {
    void fire(entt::registry &registry, WeaponComponent &weapon, ShipComponent &ship)
    {
        weapon.sprite.loadAsepriteTag("Firing");
        weapon.state = WeaponState::FIRING;
        
        auto projectile_entity = registry.create();
        auto &projectile = registry.emplace<ProjectileComponent>(projectile_entity);
        projectile.damage = 10;
        projectile.sprite = ResourceSystem::getSprite("auto_cannon_projectile");
        projectile.position = ship.position;
        projectile.type = ProjectileType::AUTO_CANNON_PROJECTILE;
        projectile.sprite.loadAsepriteTag("Fly");
    }
}

void PlayerSystem::proccessEvents(entt::registry &registry)
{
    auto view = registry.view<ShipComponent, PlayerComponent, WeaponComponent>();
    for (auto [entity, ship, player, weapon] : view.each()) {
        if (IsKeyDown(KEY_A)) {
            ship.velocity.x -= 10.f * GetFrameTime();
        }
        if (IsKeyDown(KEY_D)) {
            ship.velocity.x += 10.f * GetFrameTime();
        }
        if (IsKeyPressed(KEY_SPACE)) {
            if (weapon.state == WeaponState::FIRING)
                break;
            else
                fire(registry, weapon, ship);
        }
    }
}

void PlayerSystem::update(entt::registry &registry)
{
    auto view = registry.view<PlayerComponent, ShipComponent>();
    for (auto [entity, player, ship] : view.each()) {
        player.engine_sprite.updateTag(); 
        WeaponSystem::update(registry);
    }
}

void PlayerSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<PlayerComponent, ShipComponent>();
    for (auto [entity, player, ship] : view.each()) {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const float width = resolution.x / 10.f;
        const float height = resolution.y / 10.f;
        const Rectangle dest = {ship.position.x, ship.position.y, width, height};

        WeaponSystem::draw(registry, dest);
        player.engine_sprite.drawTagPro(dest, {0.f, 0.f}, 0.f, WHITE);
        player.ship_sprite.drawPro(0, dest, {0.f, 0.f}, 0.f, WHITE); 
    }
}
