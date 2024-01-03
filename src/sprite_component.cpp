#include "sprite_component.hpp"
#include "resource_system.hpp"

namespace {
    inline void calculateSpriteScale(SpriteComponent::Component &sprite, const float width, const float height)
    {
        const float sprite_width = sprite.sprite->getWidth();
        const float sprite_height = sprite.sprite->getWidth();

        sprite.scale = std::min(sprite_width / width, sprite_height / height);
    }
}

SpriteComponent::Component SpriteComponent::create(const std::string_view &key, const float width, const float height, const bool die_when_end)
{
    Component sprite {
        .sprite = std::make_unique<Sprite>(ResourceSystem::getSprite(key)),
        .die_when_end = die_when_end
    };
    calculateSpriteScale(sprite, width, height);
    return sprite; 
}
