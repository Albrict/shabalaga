#include "graphics.hpp"
#include "../include/glfw3.h"
#include "../include/raygui.h"
#include "../include/raymath.h"
#include "../include/style_cyber.h"
#include <algorithm>
#include <memory>
#include <vector>

namespace Graphics {
    std::vector<VideoMode> video_mode_vector;    
    std::unique_ptr<VideoMode> current_video_mode;
    float brightness = 0.f;

    RenderTexture2D target {};
}

void Graphics::init()
{
    // Init resolutions
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT); // Dirty hack to read resolutions
    InitWindow(800, 600, "Shabalaga");
    int count = 0;
    int font_size = 30;
    std::vector<Vector2> resolutions;
    const GLFWvidmode *modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
    for (size_t i = 0; i < count; ++i) {
        Vector2 resolution = { static_cast<float>(modes[i].width), static_cast<float>(modes[i].height) };
        resolutions.emplace_back(resolution);
    }
    resolutions.erase(std::unique(resolutions.begin(), resolutions.end(), Vector2Equals), resolutions.end());

    video_mode_vector.reserve(resolutions.size());
    font_size -= resolutions.size();
    for (size_t i = 0; i < resolutions.size(); ++i) {
        video_mode_vector.push_back(VideoMode(resolutions[i], font_size));
        ++font_size;
    }
    current_video_mode = std::make_unique<VideoMode>(video_mode_vector.back());
    CloseWindow();

    // Init Window and UI
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(current_video_mode->resolution.x, current_video_mode->resolution.y, "Shabalaga");
    SetWindowMinSize(video_mode_vector.front().resolution.x, video_mode_vector.front().resolution.y);
    SetTargetFPS(GetMonitorRefreshRate(0));

    GuiLoadStyleCyber();
    GuiSetStyle(DEFAULT, TEXT_SIZE, current_video_mode->font_size);
    
    // Init render texture
    target = LoadRenderTexture(current_video_mode->resolution.x, current_video_mode->resolution.y);
}

void Graphics::setVideoMode(VideoMode mode)
{
    *current_video_mode = mode;
    UnloadRenderTexture(target);
    target = LoadRenderTexture(current_video_mode->resolution.x, current_video_mode->resolution.y);
    GuiSetStyle(DEFAULT, TEXT_SIZE, current_video_mode->font_size);
}

void Graphics::setVideoMode(const int video_mode_id)
{
    *current_video_mode = video_mode_vector.at(video_mode_id);
    UnloadRenderTexture(target);
    target = LoadRenderTexture(current_video_mode->resolution.x, current_video_mode->resolution.y);
    GuiSetStyle(DEFAULT, TEXT_SIZE, current_video_mode->font_size);
}

Vector2 Graphics::getCurrentResolution()
{
    return current_video_mode->resolution;
}

void Graphics::setBrightness(const float value)
{
    brightness = value;
}

float Graphics::getBrightnessValue()
{
    return brightness;
}

int Graphics::getCurrentFontSize()
{
    return current_video_mode->font_size;
}

std::vector<Graphics::VideoMode> Graphics::getPossibleVideoModes()
{
    return video_mode_vector;
}

void Graphics::beginRender()
{
    const Vector2 current_resolution = current_video_mode->resolution;
    const float scale = std::min((GetScreenWidth() / current_resolution.x),
                            (GetScreenHeight() / current_resolution.y));
    SetMouseOffset(-(GetScreenWidth() - (current_resolution.x * scale)) * 0.5f,
                    -(GetScreenHeight() - (current_resolution.y * scale)) * 0.5f);
    SetMouseScale(1 / scale, 1 / scale);

    BeginTextureMode(target);
    ClearBackground(BLACK);
}

void Graphics::endRender()
{
    const Vector2 current_resolution = current_video_mode->resolution;
    const float scale = std::min((GetScreenWidth() / current_resolution.x),
                            (GetScreenHeight() / current_resolution.y));
    EndTextureMode();

    BeginDrawing();
    {
        ClearBackground(BLACK);
        const Rectangle source_rect = {0.0f, 0.0f,
                                    static_cast<float>(target.texture.width),
                                    static_cast<float>(-target.texture.height)};

        const Rectangle dest_rect = {
            (GetScreenWidth() - (current_resolution.x * scale)) * 0.5f,
            (GetScreenHeight() - (current_resolution.y * scale)) * 0.5f,
            current_resolution.x * scale, current_resolution.y * scale};
        DrawTexturePro(target.texture, source_rect, dest_rect, {0.f, 0.f}, 0.0f,
                    ColorBrightness(WHITE, brightness));
    }
    EndDrawing();
}

std::vector<Vector2> Graphics::getPossibleResolutions()
{
    std::vector<Vector2> resolutions;
    resolutions.reserve(video_mode_vector.size());
    for (size_t i = 0; i < video_mode_vector.size(); ++i)
        resolutions.push_back(video_mode_vector[i].resolution);
    return resolutions;
}

int Graphics::getCurrentVideoModeId()
{
    auto it = std::find(video_mode_vector.begin(), video_mode_vector.end(), *current_video_mode);
    return it - video_mode_vector.begin();
}
