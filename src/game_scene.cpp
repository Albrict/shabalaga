#include "game_scene.hpp"
#include "../include/raylib.h"
#include "player_component.hpp"
#include "ship_components.hpp"
#include "ship_system.hpp"
#include "weapon_component.hpp"
#include "resource_system.hpp"
#include "projectile_system.hpp"

GameScene::GameScene()
{
    const auto entity = registry.create();
    auto &player = registry.emplace<PlayerComponent>(entity);
    registry.emplace<ShipComponent>(entity, ShipComponent({0.f, 0.f, 0.f, 0.f}, {500.f, 500.f}, {0.f, 0.f}, 100));
    registry.emplace<ShipType>(entity, ShipType::PLAYER);
    auto &weapon = registry.emplace<WeaponComponent>(entity, ResourceSystem::getSprite("auto_cannon"), 
                                              WeaponType::AUTO_CANNON, WeaponState::IDLE);
    player.engine_sprite = ResourceSystem::getSprite("engine");
    player.ship_sprite = ResourceSystem::getSprite("ship");
    
    player.engine_sprite.loadAsepriteTag("Powering");
    weapon.sprite.loadAsepriteTag("Idle");
}

void GameScene::proccessEvents()
{
    ShipSystem::proccessEvents(registry);
}

void GameScene::update()
{
    ShipSystem::update(registry);
    ProjectileSystem::update(registry);
}

void GameScene::draw() const 
{
    ProjectileSystem::draw(registry);
    ShipSystem::draw(registry);
}
