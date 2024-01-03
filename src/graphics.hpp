#pragma once
#include "../include/raylib.h"
#include "../include/raymath.h"
#include "../include/entt.hpp"

#include <vector>

namespace Graphics {
    struct VideoMode {
        Vector2 resolution;
        int font_size;

        VideoMode(const Vector2 resolution, const int font_size)
            : resolution(resolution), font_size(font_size) {}

        VideoMode operator=(VideoMode &mode)
        {
            if (this == &mode)
                return *this;
            this->font_size = mode.font_size;
            this->resolution = mode.resolution;
            return *this;
        }
        friend bool operator==(const VideoMode &rhs, const VideoMode &lhs)
        {
            return ((rhs.font_size == lhs.font_size) && (Vector2Equals(rhs.resolution, lhs.resolution))); 
        }
    };
    void init();
    
    void beginRender();
    void endRender();

    void setVideoMode(VideoMode mode);
    void setVideoMode(const int video_mode_id);
    [[nodiscard]] int getCurrentVideoModeId();
    [[nodiscard]] std::vector<VideoMode> getPossibleVideoModes();
    [[nodiscard]] std::vector<Vector2> getPossibleResolutions();
    [[nodiscard]] Vector2 getCurrentResolution();
    
    void setBrightness(const float value);
    [[nodiscard]] float getBrightnessValue();
    [[nodiscard]] int getCurrentFontSize();      
};
