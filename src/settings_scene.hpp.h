#pragma once
#include "scene.hpp"
#include "../include/glfw3.h"

class SettingsScene final : public Scene {
public:
    SettingsScene();
    ~SettingsScene() = default;
    
    void proccessEvents() override;
    void update() override;
    void draw() const override;
private:
    void initWidgets();
    Rectangle initBrightnessSlider(const Rectangle panel_rect);
    void initResolutionDropdownBox(const Rectangle slider_rect);
    void initButtons(const Rectangle panel_rect);

    static void saveValueCallback(entt::any data);
    static void backToMainMenuCallback(entt::any data);
private:
    int choosen_resolution = 0;
    int m_resolutions_count = 0;
    float brightness_value = 0.f;
    const GLFWvidmode *m_modes = nullptr;
    entt::registry m_widget_registry {}; 
    std::vector<Vector2> resolution_vector {};
    std::unique_ptr<std::string> resolution_list;
};
