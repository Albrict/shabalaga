#pragma once
#include "../include/entt.hpp"
#include "aseprite.hpp"

namespace ResourceSystem {
    void loadTexture(const char *path, const std::string_view &key);
    Texture2D getTexture(const std::string_view &key);

    void loadAseprite(const char *path, const std::string_view &key);
    Aseprite::Aseprite getAseprite(const std::string_view &key);
    
    Aseprite::AsepriteTag getAsepriteTag(const std::string_view &key, const unsigned int tag_id);
    Aseprite::AsepriteSlice getAsepriteSlice(const std::string_view &key, const unsigned int slice_id);

    void loadSound(const char *path, const std::string_view &key);
    Sound getSound(const std::string_view &key);
    
    void loadMusic(const char *path, const std::string_view &key);
    Music getMusic(const std::string_view &key);

    void unloadResources();
};
