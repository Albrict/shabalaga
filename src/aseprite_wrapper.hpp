#pragma once
#include "../include/raylib-aseprite.hpp"

class Sprite {
public:
    Sprite(const char *path_to_aseprite); 
    ~Sprite();
    
    bool loadAsepriteTag(const char *name);
    void draw(const int frame, const int x, const int y, const Color tint) const;
    void drawPro(const int frame, const Rectangle dest, const Vector2 origin, const float rotation, const Color tint);
    void drawV(const int frame, const Vector2 position, const Color tint);
    void drawEx(const int frame, const Vector2 position, const float rotation, const float scale, const Color tint);

    void drawTag(const int x, const int y, const Color tint) const;
    void drawTagPro(const Rectangle dest, const Vector2 origin, const float rotation, const Color tint);
    void drawTagV(const Vector2 position, const Color tint);
    void drawTagEx(const Vector2 position, const float rotation, const float scale, const Color tint); 

    void updateTag();
private:
    Aseprite *aseprite_ptr = nullptr;
    AsepriteTag *aseprite_tag_ptr = nullptr;
};
