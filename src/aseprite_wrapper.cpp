#include <stdexcept>
#define RAYLIB_ASEPRITE_IMPLEMENTATION
#include "../include/raylib-aseprite.hpp"
#undef RAYLIB_ASEPRITE_IMPLEMENTATION
#include "aseprite_wrapper.hpp"

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
    if (aseprite_tag_ptr)
        free(aseprite_tag_ptr);
}

bool Sprite::loadAsepriteTag(const char *name)
{
    aseprite_tag_ptr = static_cast<AsepriteTag*>(malloc(sizeof(struct AsepriteTag)));
    *aseprite_tag_ptr = LoadAsepriteTag(*aseprite_ptr, name);
    if (!IsAsepriteTagReady(*aseprite_tag_ptr)) 
        return false;
    else
        return true;
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
    DrawAsepriteTag(*aseprite_tag_ptr, x, y, tint);
}
void Sprite::drawTagPro(const Rectangle dest, const Vector2 origin, const float rotation, const Color tint)
{
    DrawAsepriteTagPro(*aseprite_tag_ptr, dest, origin, rotation, tint);
}

void Sprite::drawTagV(const Vector2 position, const Color tint)
{
    DrawAsepriteTagV(*aseprite_tag_ptr, position, tint);
}

void Sprite::drawTagEx(const Vector2 position, const float rotation, const float scale, const Color tint)
{
    DrawAsepriteTagEx(*aseprite_tag_ptr, position, rotation, scale, tint);
}

void Sprite::updateTag()
{
    UpdateAsepriteTag(aseprite_tag_ptr);
}
