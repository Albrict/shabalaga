#pragma once
#include "../include/entt.hpp"
#include "aseprite_wrapper.hpp"

namespace ResourceSystem {
    void loadTexture(const char *path, const std::string_view &key);
    Texture2D getTexture(const std::string_view &key);

    void loadAseprite(const char *path, const std::string_view &key);
    std::shared_ptr<AsepriteWrapper> getAseprite(const std::string_view &key);

   // void loadAsepriteTag(const char *path, const std::string_view &key);
   // AsepriteTag getAsepriteTag(const std::string_view &key);
    void unloadResources();
};
