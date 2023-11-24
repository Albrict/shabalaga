#pragma once
#include "../include/entt.hpp"
#include "aseprite_wrapper.hpp"

namespace ResourceSystem {
    void loadTexture(const char *path, const std::string_view &key);
    Texture2D getTexture(const std::string_view &key);

    void loadSprite(const char *path, const std::string_view &key);
    std::shared_ptr<Sprite> getSprite(const std::string_view &key);

   // void loadAsepriteTag(const char *path, const std::string_view &key);
   // AsepriteTag getAsepriteTag(const std::string_view &key);
    void unloadResources();
};
