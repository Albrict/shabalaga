#pragma once
#include "graphics.hpp"
#include "scene.hpp"

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
    Rectangle initSoundSlider(const Rectangle brightness_slider);
    
    void readSettings();
    void initFullscreenCheckBox(const Rectangle sound_slider_rect);
    void initResolutionDropdownBox(const Rectangle slider_rect);
    void initButtons(const Rectangle panel_rect);
        
    static void saveValueCallback(entt::any data);
    static void backToMainMenuCallback(entt::any data);
private:
    int choosen_resolution = Graphics::getCurrentVideoModeId();
    float brightness_value = Graphics::getBrightnessValue();
    float general_volume = 0.5f;
    bool is_fullscreen = false;    
    std::vector<Vector2> resolution_vector {};
    std::unique_ptr<std::string> resolution_list {};
};
