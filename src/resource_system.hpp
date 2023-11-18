#pragma once
#include "../include/entt.hpp"
#include <filesystem>
#include "../include/raylib.h"

namespace ResourceSystem {
    void loadTexture(const std::filesystem::path &path, const std::string_view &key);
    void unloadTextures();
    Texture2D getTexture(const std::string_view &key);
};
