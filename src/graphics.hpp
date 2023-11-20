#pragma once
#include "../include/raylib.h"
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
    };
    void init();

    void setVideoMode(VideoMode mode);
    void setVideoMode(const int video_mode_id);
    std::vector<VideoMode> getPossibleVideoModes();
    std::vector<Vector2> getPossibleResolutions();
    Vector2 getCurrentResolution();
    
    void setBrightness(const float value);
    float getBrightnessValue();

    int getCurrentFontSize();      
    void beginRender();
    void endRender();
};
