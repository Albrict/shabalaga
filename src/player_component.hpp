#pragma once
#include "aseprite_wrapper.hpp"
#include <array>
#include <memory>

struct PlayerComponent {
    std::shared_ptr<Sprite> ship_sprite;
    std::shared_ptr<Sprite> engine_sprite;

    PlayerComponent(std::shared_ptr<Sprite> ship, std::shared_ptr<Sprite> engine) 
        : ship_sprite(ship),
        engine_sprite(engine) 
    {
        engine->loadAsepriteTag("Powering");
    }
};
