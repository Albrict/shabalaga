#pragma once
#include "../include/raylib-aseprite.hpp"
#include <vector>

class Sprite {
public:
    Sprite() = default; 
    Sprite(Aseprite *aseprite);
    ~Sprite();
    
    float getWidth();
    float getHeight();
    Rectangle getSlice(const char *slice_name);
    void setFrame(const unsigned int frame) noexcept;
    void loadAseprite(Aseprite *aseprite);
    bool loadAsepriteTag(const char *name);
    void draw(const int x, const int y, const Color tint) const;
    void drawPro(const Rectangle dest, const Vector2 origin, const float rotation, const Color tint) const;
    void drawV(const Vector2 position, const Color tint) const;
    void drawEx(const Vector2 position, const float rotation, const float scale, const Color tint) const;

    void drawTag(const int x, const int y, const Color tint) const;
    void drawTagPro(const Rectangle dest, const Vector2 origin, const float rotation, const Color tint) const;
    void drawTagV(const Vector2 position, const Color tint) const;
    void drawTagEx(const Vector2 position, const float rotation, const float scale, const Color tint) const;

    void updateTag();
    
    int getCurrentTagId() const noexcept;
    int getCurrentTagFrame() const noexcept;
    int getCurrentSpriteFrame() const noexcept;
    int getFrames() const noexcept;
    void setTagFrame(const int frame);
private:
    std::vector<AsepriteTag*> tags {};
    std::vector<AsepriteSlice*> slices {};
    Aseprite *aseprite_ptr = nullptr;
    AsepriteTag *current_tag = nullptr;
    int current_tag_id = 0;
    int current_sprite_frame = 0;
};

namespace AsepriteWrapper {
    // Allocate aseprite struct and load it
    Aseprite *loadAseprite(const char *path);
    // Free aseprite struct and unload resource
    void UnloadAseprite(Aseprite *aseprite);
    
    std::vector<AsepriteTag*> loadTags(Aseprite *aseprite);
    std::vector<AsepriteSlice*> loadSlices(Aseprite *aseprite); 
};
