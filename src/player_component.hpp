#pragma once
#include "sprite.hpp"
#include <memory>

struct PlayerComponent {
    std::shared_ptr<Sprite> ship_sprite;
    std::shared_ptr<Sprite> engine_sprite;
    std::shared_ptr<Sprite> weapon_sprite;

    PlayerComponent(std::shared_ptr<Sprite> ship, std::shared_ptr<Sprite> engine, std::shared_ptr<Sprite> weapon) 
        : ship_sprite(ship),
        engine_sprite(engine),
        weapon_sprite(weapon)
    {
        engine_sprite->loadAsepriteTag("Powering");
        weapon_sprite->loadAsepriteTag("Idle");
    }
};
