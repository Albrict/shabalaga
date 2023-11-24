#include <stdexcept>
#define RAYLIB_ASEPRITE_IMPLEMENTATION
#include "../include/raylib-aseprite.hpp"
#undef RAYLIB_ASEPRITE_IMPLEMENTATION
#include "sprite.hpp"
#include <cassert>

Sprite::Sprite(const char *path_to_aseprite)
{
    aseprite_ptr = static_cast<Aseprite*>(malloc(sizeof(struct Aseprite)));
    *aseprite_ptr = LoadAseprite(path_to_aseprite); 
    if (!IsAsepriteReady(*aseprite_ptr)) 
        throw std::exception();
}

Sprite::~Sprite()
{
    if (aseprite_ptr) {
        UnloadAseprite(*aseprite_ptr); 
        free(aseprite_ptr);
    }
    if (current_tag)
        free(current_tag);
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

void Sprite::drawPro(const int frame, const Rectangle dest, const Vector2 origin, const float rotation, const Color tint)
{
    DrawAsepritePro(*aseprite_ptr, frame, dest, origin, rotation, tint);
}

void Sprite::drawV(const int frame, const Vector2 position, const Color tint)
{
    DrawAsepriteV(*aseprite_ptr, frame, position, tint);
}

void Sprite::drawEx(const int frame, const Vector2 position, const float rotation, const float scale, const Color tint)
{
    DrawAsepriteEx(*aseprite_ptr, frame, position, rotation, scale, tint);
}

void Sprite::drawTag(const int x, const int y, const Color tint) const
{
    DrawAsepriteTag(*current_tag, x, y, tint);
}
void Sprite::drawTagPro(const Rectangle dest, const Vector2 origin, const float rotation, const Color tint)
{
    DrawAsepriteTagPro(*current_tag, dest, origin, rotation, tint);
}

void Sprite::drawTagV(const Vector2 position, const Color tint)
{
    DrawAsepriteTagV(*current_tag, position, tint);
}

void Sprite::drawTagEx(const Vector2 position, const float rotation, const float scale, const Color tint)
{
    DrawAsepriteTagEx(*current_tag, position, rotation, scale, tint);
}

void Sprite::updateTag()
{
    UpdateAsepriteTag(current_tag);
}
