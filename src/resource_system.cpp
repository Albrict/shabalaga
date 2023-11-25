#define RAYLIB_ASEPRITE_IMPLEMENTATION
#include "../include/raylib-aseprite.hpp"
#undef RAYLIB_ASEPRITE_IMPLEMENTATION
#include "sprite.hpp"
#include "resource_system.hpp"

#include <cassert>
#include <stdexcept>

namespace {
    std::map<std::string_view, Texture2D> texture_storage;
    std::map<std::string_view, Aseprite*> aseprite_storage;
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
    for (auto aseprite : aseprite_storage) {
        UnloadAseprite(*aseprite.second);
        free(aseprite.second);
    }
}

Texture2D ResourceSystem::getTexture(const std::string_view &key)
{
    return texture_storage[key];
}


void ResourceSystem::loadSprite(const char *path, const std::string_view &key)
{
    Aseprite *aseprite = static_cast<Aseprite*>(malloc(sizeof(Aseprite)));
    *aseprite = LoadAseprite(path);
    aseprite_storage.insert({key, aseprite});
}

Sprite ResourceSystem::getSprite(const std::string_view &key)
{
    Sprite sprite(aseprite_storage[key]);
    return sprite;

}
Sprite::~Sprite()
{
    if (current_tag) {
        free(current_tag);
        current_tag = nullptr;
    }
}

bool Sprite::loadAsepriteTag(const char *name)
{
    AsepriteTag *aseprite_tag_ptr = static_cast<AsepriteTag*>(malloc(sizeof(struct AsepriteTag)));
    *aseprite_tag_ptr = LoadAsepriteTag(*aseprite_ptr, name);
    if (!IsAsepriteTagReady(*aseprite_tag_ptr)) {
        return false;
    } else {
        if (current_tag)
            free(current_tag);
        current_tag = aseprite_tag_ptr;
        return true;
    }
}

void Sprite::draw(const int frame, const int x, const int y, const Color tint) const
{
    DrawAseprite(*aseprite_ptr, frame, x, y, tint); 
}

void Sprite::drawPro(const int frame, const Rectangle dest, const Vector2 origin, const float rotation, const Color tint) const
{
    DrawAsepritePro(*aseprite_ptr, frame, dest, origin, rotation, tint);
}

void Sprite::drawV(const int frame, const Vector2 position, const Color tint) const
{
    DrawAsepriteV(*aseprite_ptr, frame, position, tint);
}

void Sprite::drawEx(const int frame, const Vector2 position, const float rotation, const float scale, const Color tint) const
{
    DrawAsepriteEx(*aseprite_ptr, frame, position, rotation, scale, tint);
}

void Sprite::drawTag(const int x, const int y, const Color tint) const
{
    DrawAsepriteTag(*current_tag, x, y, tint);
}

void Sprite::drawTagPro(const Rectangle dest, const Vector2 origin, const float rotation, const Color tint) const
{
    DrawAsepriteTagPro(*current_tag, dest, origin, rotation, tint);
}

void Sprite::drawTagV(const Vector2 position, const Color tint) const
{
    DrawAsepriteTagV(*current_tag, position, tint);
}

void Sprite::drawTagEx(const Vector2 position, const float rotation, const float scale, const Color tint) const
{
    DrawAsepriteTagEx(*current_tag, position, rotation, scale, tint);
}

void Sprite::updateTag()
{
    UpdateAsepriteTag(current_tag);
}

int Sprite::getCurrentTagFrame() const noexcept 
{
    return current_tag->currentFrame;
}

void Sprite::setTagFrame(const int frame) 
{
    current_tag->currentFrame = frame; 
}
