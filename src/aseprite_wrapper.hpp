#pragma once
#include "../include/raylib-aseprite.hpp"
#include <cstdlib>

class AsepriteWrapper {
public:
    friend class AsepriteTagWrapper;
    AsepriteWrapper(const char *path_to_aseprite); 
    ~AsepriteWrapper();
    
    void draw(const int frame, const int x, const int y, const Color tint) const;
    void drawPro(const int frame, const Rectangle dest, const Vector2 origin, const float rotation, const Color tint);
    void drawV(const int frame, const Vector2 position, const Color tint);
    void drawEx(const int frame, const Vector2 position, const float rotation, const float scale, const Color tint); 
private:
    Aseprite *aseprite_ptr = nullptr;
};

class AsepriteTagWrapper {
public:
    AsepriteTagWrapper(const AsepriteWrapper &aseprite, const char *tag_name);
    ~AsepriteTagWrapper()
    {
        if (aseprite_tag_ptr)
            free(aseprite_tag_ptr);
    }

    void draw(const int x, const int y, const Color tint) const;
    void drawPro(const Rectangle dest, const Vector2 origin, const float rotation, const Color tint);
    void drawV(const Vector2 position, const Color tint);
    void drawEx(const Vector2 position, const float rotation, const float scale, const Color tint); 
private:
    AsepriteTag *aseprite_tag_ptr = nullptr;
};
