#pragma once
#include "../include/entt.hpp"
#include "sprite.hpp"

namespace ResourceSystem {
    void loadTexture(const char *path, const std::string_view &key);
    Texture2D getTexture(const std::string_view &key);

    void loadSprite(const char *path, const std::string_view &key);
    Sprite getSprite(const std::string_view &key);

    void unloadResources();
};
