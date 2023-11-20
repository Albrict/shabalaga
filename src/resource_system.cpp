#include "resource_system.hpp"
#include <iostream>

namespace {
    std::map<std::string_view, Texture2D> texture_storage;
};

void ResourceSystem::loadTexture(const std::filesystem::path &path, const std::string_view &key)
{
    Texture2D texture = LoadTexture(path.c_str());
    texture_storage.insert({key, texture});
}

void ResourceSystem::unloadTextures()
{
    for (auto texture : texture_storage)
        UnloadTexture(texture.second);
}

Texture2D ResourceSystem::getTexture(const std::string_view &key)
{
    return texture_storage.at(key);
}
