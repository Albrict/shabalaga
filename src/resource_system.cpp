#include "sprite.hpp"
#include "resource_system.hpp"

namespace ResourceSystem {
    std::map<std::string_view, Texture2D> texture_storage;
    std::map<std::string_view, Aseprite*> aseprite_storage;
    std::map<std::string_view, Sound> sound_storage;
    std::map<std::string_view, Music> music_storage;

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
    for (auto sound : sound_storage)
        UnloadSound(sound.second);
    for (auto music : music_storage)
        UnloadMusicStream(music.second);
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

void ResourceSystem::loadSound(const char *path, const std::string_view &key)
{
    Sound sound = LoadSound(path);
    if (IsSoundReady(sound))
        sound_storage.insert({key, sound});
}

Sound ResourceSystem::getSound(const std::string_view &key)
{
    return sound_storage.at(key);
}

void ResourceSystem::loadMusic(const char *path, const std::string_view &key)
{
    Music music = LoadMusicStream(path);
    if (IsMusicReady(music))
        music_storage.insert({key, music});  
}

Music ResourceSystem::getMusic(const std::string_view &key)
{
    return music_storage.at(key);   
}
