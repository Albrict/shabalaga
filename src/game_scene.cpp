#include "game_scene.hpp"
#include "../include/raylib.h"
#include "player_component.hpp"
#include "ship_components.hpp"
#include "ship_system.hpp"
#include "resource_system.hpp"

GameScene::GameScene()
{
    const auto entity = registry.create();
    registry.emplace<PlayerComponent>(entity, PlayerComponent(ResourceSystem::getTexture("ship_spritesheet")));
    registry.emplace<ShipComponent>(entity, ShipComponent({0.f, 0.f, 0.f, 0.f}, {500.f, 500.f}, {0.f, 0.f}, 100));
    registry.emplace<ShipType>(entity, ShipType::PLAYER);
}

void GameScene::proccessEvents()
{
    ShipSystem::proccessEvents(registry);
}

void GameScene::update()
{
    ShipSystem::update(registry);
}

void GameScene::draw() const 
{
    ShipSystem::draw(registry);
}
