#include <stdexcept>
#define RAYLIB_ASEPRITE_IMPLEMENTATION
#include "../include/raylib-aseprite.hpp"
#undef RAYLIB_ASEPRITE_IMPLEMENTATION
#include "aseprite_wrapper.hpp"

AsepriteWrapper::AsepriteWrapper(const char *path_to_aseprite)
{
    aseprite_ptr = static_cast<Aseprite*>(malloc(sizeof(struct Aseprite)));
    *aseprite_ptr = LoadAseprite(path_to_aseprite); 
    if (!IsAsepriteReady(*aseprite_ptr)) 
        throw std::exception();
}

AsepriteWrapper::~AsepriteWrapper()
{
    if (aseprite_ptr) {
        UnloadAseprite(*aseprite_ptr); 
        free(aseprite_ptr);
    }
}

void AsepriteWrapper::draw(const int frame, const int x, const int y, const Color tint) const
{
    DrawAseprite(*aseprite_ptr, frame, x, y, tint); 
}

void AsepriteWrapper::drawPro(const int frame, const Rectangle dest, const Vector2 origin, const float rotation, const Color tint)
{
    DrawAsepritePro(*aseprite_ptr, frame, dest, origin, rotation, tint);
}

void AsepriteWrapper::drawV(const int frame, const Vector2 position, const Color tint)
{
    DrawAsepriteV(*aseprite_ptr, frame, position, tint);
}

void AsepriteWrapper::drawEx(const int frame, const Vector2 position, const float rotation, const float scale, const Color tint)
{
    DrawAsepriteEx(*aseprite_ptr, frame, position, rotation, scale, tint);
}

AsepriteTagWrapper::AsepriteTagWrapper(const AsepriteWrapper &aseprite, const char *tag_name)
{
    aseprite_tag_ptr = static_cast<AsepriteTag*>(malloc(sizeof(struct AsepriteTag)));
    *aseprite_tag_ptr = LoadAsepriteTag(*aseprite.aseprite_ptr, tag_name);
    if (!IsAsepriteTagReady(*aseprite_tag_ptr)) 
        throw std::exception();
}

void AsepriteTagWrapper::draw(const int x, const int y, const Color tint) const
{
    DrawAsepriteTag(*aseprite_tag_ptr, x, y, tint);
}
void AsepriteTagWrapper::drawPro(const Rectangle dest, const Vector2 origin, const float rotation, const Color tint)
{
    DrawAsepriteTagPro(*aseprite_tag_ptr, dest, origin, rotation, tint);
}

void AsepriteTagWrapper::drawV(const Vector2 position, const Color tint)
{
    DrawAsepriteTagV(*aseprite_tag_ptr, position, tint);
}

void AsepriteTagWrapper::drawEx(const Vector2 position, const float rotation, const float scale, const Color tint)
{
    DrawAsepriteTagEx(*aseprite_tag_ptr, position, rotation, scale, tint);
}
