#include "resource_system.hpp"

namespace {
    std::map<std::string_view, Texture2D> texture_storage;
    std::map<std::string_view, std::shared_ptr<AsepriteWrapper>> aseprite_storage;
};

void ResourceSystem::loadTexture(const char *path, const std::string_view &key)
{
    Texture2D texture = LoadTexture(path);
    texture_storage.insert({key, texture});
}

void ResourceSystem::unloadResources()
{
    for (auto texture : texture_storage)
        UnloadTexture(texture.second);
    aseprite_storage.clear();
}

Texture2D ResourceSystem::getTexture(const std::string_view &key)
{
    return texture_storage.at(key);
}


void ResourceSystem::loadAseprite(const char *path, const std::string_view &key)
{
    std::shared_ptr<AsepriteWrapper> sprite = std::make_shared<AsepriteWrapper>(path);
    aseprite_storage.insert({key, sprite});
}

std::shared_ptr<AsepriteWrapper> ResourceSystem::getAseprite(const std::string_view &key)
{
    return aseprite_storage[key];
}
