#include <stdexcept>
#define RAYLIB_ASEPRITE_IMPLEMENTATION
#include "../include/raylib-aseprite.hpp"
#undef RAYLIB_ASEPRITE_IMPLEMENTATION
#include "sprite.hpp"
#include <cstdlib>

Sprite::Sprite(Aseprite *aseprite)
    : aseprite_ptr(aseprite) 
{
    for (size_t i = 0; i < aseprite_ptr->ase->tag_count; ++i) {
        AsepriteTag tag = LoadAsepriteTagFromIndex(*aseprite_ptr, i);
        if (IsAsepriteTagReady(tag)) {
            AsepriteTag *allocated_tag = static_cast<AsepriteTag*>(malloc(sizeof(AsepriteTag)));
            *allocated_tag = tag;
            tags.push_back(allocated_tag);  
        }        
    }   
    for (size_t i = 0; i < aseprite_ptr->ase->slice_count; ++i) {
        AsepriteSlice slice = LoadAsperiteSliceFromIndex(*aseprite_ptr, i);
        if (IsAsepriteSliceReady(slice)) {
            AsepriteSlice *allocated_slice = static_cast<AsepriteSlice*>(malloc(sizeof(AsepriteSlice)));
            *allocated_slice = slice;
            slices.push_back(allocated_slice);
        } 
    }
}

Sprite::~Sprite()
{
    current_tag = nullptr;
    if (!tags.empty()) {
        for (auto tag : tags) 
            free(tag);
    }
    if (!slices.empty()) {
        for (auto slice : slices)
            free(slice);
    }
}

float Sprite::getWidth()
{
    return GetAsepriteWidth(*aseprite_ptr);
}

float Sprite::getHeight()
{
    return GetAsepriteHeight(*aseprite_ptr);
}

Rectangle Sprite::getSlice(const char *slice_name)
{
    for (auto slice : slices) {
        if (TextIsEqual(slice_name, slice->name))
            return slice->bounds;
    }
    throw std::out_of_range("Can't find slice!\n");
}

void Sprite::loadAseprite(Aseprite *aseprite)
{
    aseprite_ptr = aseprite;
    for (size_t i = 0; i < aseprite_ptr->ase->tag_count; ++i) {
        AsepriteTag tag = LoadAsepriteTagFromIndex(*aseprite_ptr, i);
        if (IsAsepriteTagReady(tag)) {
            AsepriteTag *allocated_tag = static_cast<AsepriteTag*>(malloc(sizeof(AsepriteTag)));
            *allocated_tag = tag;
            tags.push_back(allocated_tag);  
        }        
    }
    for (size_t i = 0; i < aseprite_ptr->ase->slice_count; ++i) {
        AsepriteSlice slice = LoadAsperiteSliceFromIndex(*aseprite_ptr, i);
        if (IsAsepriteSliceReady(slice)) {
            AsepriteSlice *allocated_slice = static_cast<AsepriteSlice*>(malloc(sizeof(AsepriteSlice)));
            *allocated_slice = slice;
            slices.push_back(allocated_slice);
        } 
    }
}

bool Sprite::loadAsepriteTag(const char *name)
{
    for (size_t i = 0; i < tags.size(); ++i) {
        if (TextIsEqual(name, tags[i]->name)) {
            current_tag = tags[i];
            current_tag_id = i;
            return true;
        }
    }
    return false;
//    AsepriteTag *aseprite_tag_ptr = static_cast<AsepriteTag*>(malloc(sizeof(struct AsepriteTag)));
//    *aseprite_tag_ptr = LoadAsepriteTag(*aseprite_ptr, name);
//    if (!IsAsepriteTagReady(*aseprite_tag_ptr)) {
//        return false;
//    } else {
//        if (current_tag)
//            free(current_tag);
//        current_tag = aseprite_tag_ptr;
//        return true;
//    }
}

void Sprite::draw(const int x, const int y, const Color tint) const
{
    DrawAseprite(*aseprite_ptr, current_sprite_frame, x, y, tint); 
}

void Sprite::drawPro(const Rectangle dest, const Vector2 origin, const float rotation, const Color tint) const
{
    DrawAsepritePro(*aseprite_ptr, current_sprite_frame, dest, origin, rotation, tint);
}

void Sprite::drawV(const Vector2 position, const Color tint) const
{
    DrawAsepriteV(*aseprite_ptr, current_sprite_frame, position, tint);
}

void Sprite::drawEx(const Vector2 position, const float rotation, const float scale, const Color tint) const
{
    DrawAsepriteEx(*aseprite_ptr, current_sprite_frame, position, rotation, scale, tint);
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

int Sprite::getFrames() const noexcept
{
    return current_tag->aseprite.ase->frames->cel_count + 1;
}

int Sprite::getCurrentTagId() const noexcept
{
    return current_tag_id; 
}

int Sprite::getCurrentTagFrame() const noexcept 
{
    return GetAsepriteTagFrame(*current_tag);
}

int Sprite::getCurrentSpriteFrame() const noexcept
{
    return current_sprite_frame;
}

void Sprite::setFrame(const unsigned int frame) noexcept
{
    current_sprite_frame = frame;
}

void Sprite::setTagFrame(const int frame) 
{
    current_tag->currentFrame = frame; 
}

Aseprite *AsepriteWrapper::loadAseprite(const char *path)
{
    Aseprite *sprite = static_cast<Aseprite*>(malloc(sizeof(Aseprite)));
    *sprite = LoadAseprite(path); 
    if (sprite) {
        if (IsAsepriteReady(*sprite))
            return sprite;
        else
            return nullptr;
    }
    return nullptr;
}

void AsepriteWrapper::UnloadAseprite(Aseprite *aseprite)
{
    if (aseprite) {
        UnloadAseprite(*aseprite);
        free(aseprite);
    }
}

std::vector<AsepriteTag*> AsepriteWrapper::loadTags(Aseprite *aseprite)
{
    std::vector<AsepriteTag*> tags;
    for (size_t i = 0; i < aseprite->ase->tag_count; ++i) {
        AsepriteTag tag = LoadAsepriteTagFromIndex(*aseprite, i);
        if (IsAsepriteTagReady(tag)) {
            AsepriteTag *allocated_tag = static_cast<AsepriteTag*>(malloc(sizeof(AsepriteTag)));
            *allocated_tag = tag;
            tags.push_back(allocated_tag);
        }
    }
    return tags;
}

std::vector<AsepriteSlice*> AsepriteWrapper::loadSlices(Aseprite *aseprite)
{
    std::vector<AsepriteSlice*> slices;
    for (size_t i = 0; i < aseprite->ase->slice_count; ++i) {
        AsepriteSlice slice = LoadAsperiteSliceFromIndex(*aseprite, i);
        if (IsAsepriteSliceReady(slice)) {
            AsepriteSlice *allocated_slice = static_cast<AsepriteSlice*>(malloc(sizeof(AsepriteSlice)));
            *allocated_slice = slice;
            slices.push_back(allocated_slice);
        } 
    }
    return slices;
}
