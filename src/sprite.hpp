#pragma once
#include "../include/raylib-aseprite.hpp"
#include <vector>

class Sprite {
public:
    Sprite() = default; 
    Sprite(Aseprite *aseprite)
        : aseprite_ptr(aseprite) {}
    ~Sprite();
    
    bool loadAsepriteTag(const char *name);
    void draw(const int frame, const int x, const int y, const Color tint) const;
    void drawPro(const int frame, const Rectangle dest, const Vector2 origin, const float rotation, const Color tint) const;
    void drawV(const int frame, const Vector2 position, const Color tint) const;
    void drawEx(const int frame, const Vector2 position, const float rotation, const float scale, const Color tint) const;

    void drawTag(const int x, const int y, const Color tint) const;
    void drawTagPro(const Rectangle dest, const Vector2 origin, const float rotation, const Color tint) const;
    void drawTagV(const Vector2 position, const Color tint) const;
    void drawTagEx(const Vector2 position, const float rotation, const float scale, const Color tint) const;

    void updateTag();

    int getCurrentTagFrame() const noexcept;
    void setTagFrame(const int frame);
private:
    Aseprite *aseprite_ptr = nullptr;
    AsepriteTag *current_tag = nullptr;
};
