#include "sprite.hpp"
#include "resource_system.hpp"

namespace ResourceSystem {
    struct SpriteResource {
        Aseprite *aseprite;
        std::shared_ptr<std::vector<AsepriteTag*>> tags; 
        std::shared_ptr<std::vector<AsepriteSlice*>> slices;

        ~SpriteResource()
        {
            AsepriteWrapper::UnloadAseprite(aseprite);
        }
    };
    std::map<std::string_view, Texture2D> texture_storage;
    std::map<std::string_view, Aseprite*> aseprite_storage;
};

void ResourceSystem::loadTexture(const char *path, const std::string_view &key)
{
    Texture2D texture = LoadTexture(path);
    texture_storage.insert_or_assign(key, texture);
}

void ResourceSystem::unloadResources()
{
    for (auto texture : texture_storage)
        UnloadTexture(texture.second);
    for (auto aseprite : aseprite_storage)
        AsepriteWrapper::UnloadAseprite(aseprite.second);
}

Texture2D ResourceSystem::getTexture(const std::string_view &key)
{
    return texture_storage.at(key);
}

void ResourceSystem::loadSprite(const char *path, const std::string_view &key)
{
    Aseprite *aseprite = AsepriteWrapper::loadAseprite(path);
    if (aseprite)
        aseprite_storage.insert({key, aseprite});
}

Aseprite* ResourceSystem::getSprite(const std::string_view &key)
{
    return aseprite_storage.at(key);
}
