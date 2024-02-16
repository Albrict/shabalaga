#include "aseprite.hpp"
#include "resource_system.hpp"

namespace ResourceSystem {
    struct AsepriteResource {
        Aseprite::Aseprite aseprite;
        std::vector<Aseprite::AsepriteTag> tags;
        std::vector<Aseprite::AsepriteSlice> slices;
    };
    
    std::map<std::string_view, Texture2D> texture_storage;
    std::map<std::string_view, AsepriteResource> aseprite_storage;
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
    for (auto &texture : texture_storage)
        UnloadTexture(texture.second);
    for (auto &aseprite : aseprite_storage)
        Aseprite::UnloadAseprite(aseprite.second.aseprite);
    for (auto &sound : sound_storage)
        UnloadSound(sound.second);
    for (auto &music : music_storage)
        UnloadMusicStream(music.second);
}

Texture2D ResourceSystem::getTexture(const std::string_view &key)
{
    return texture_storage.at(key);
}

void ResourceSystem::loadAseprite(const char *path, const std::string_view &key)
{
    Aseprite::Aseprite aseprite = Aseprite::LoadAseprite(path);
    if (Aseprite::IsAsepriteReady(aseprite)) {
        AsepriteResource resource;
        const size_t tag_count = Aseprite::GetAsepriteTagCount(aseprite);
        const size_t slice_count = Aseprite::GetAsepriteSliceCount(aseprite);

        if (tag_count > 0) {
            for (size_t i = 0; i < tag_count; ++i) {
                Aseprite::AsepriteTag tag = Aseprite::LoadAsepriteTagFromIndex(aseprite, i);
                resource.tags.push_back(tag);
            }
        }
        if (slice_count > 0) {
            for (size_t i = 0; i < slice_count; ++i) {
                Aseprite::AsepriteSlice slice = Aseprite::LoadAsperiteSliceFromIndex(aseprite, i); 
                resource.slices.push_back(slice);
            }
        }
        resource.aseprite = aseprite;
        aseprite_storage.insert({key, resource});
    }
}

Aseprite::Aseprite ResourceSystem::getAseprite(const std::string_view &key)
{
    return aseprite_storage.at(key).aseprite;
}

Aseprite::AsepriteTag ResourceSystem::getAsepriteTag(const std::string_view &key, const unsigned int tag_id)
{
    return aseprite_storage.at(key).tags.at(tag_id);
}

Aseprite::AsepriteSlice ResourceSystem::getAsepriteSlice(const std::string_view &key, const unsigned int slice_id)
{
    return aseprite_storage.at(key).slices.at(slice_id);
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
