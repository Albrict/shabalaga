#include "settings_scene.hpp"
#include "../include/raygui.h"
#include "../include/raymath.h"
#include "background_component.hpp"
#include "background_system.hpp"
#include "graphics.hpp"
#include "widget_components.hpp"
#include "widget_system.hpp"
#include "game.hpp"

SettingsScene::SettingsScene()
{
    resolution_list = std::make_unique<std::string>();
    resolution_vector = Graphics::getPossibleResolutions();
    choosen_resolution = resolution_vector.size() - 1;
    initWidgets();

    const auto background_entity = m_widget_registry.create();
    m_widget_registry.emplace<BackgroundComponent>(background_entity, 
                                                 BackgroundComponent(ResourceSystem::getTexture("menu_background"), 0.f, 0.f));
}

void SettingsScene::initWidgets()
{
    const Vector2 resolution = Graphics::getCurrentResolution();
    const float panel_width = resolution.x / 2.f;
    const float panel_height = resolution.y / 2.f;
    const float panel_x = resolution.x / 2.f - panel_width / 2.f;
    const float panel_y = resolution.y / 2.f - panel_height / 2.f;
    const Rectangle panel_rect = {panel_x, panel_y, panel_width, panel_height};

    Rectangle slider_rect = initBrightnessSlider(panel_rect);
    initResolutionDropdownBox(slider_rect);
    initButtons(panel_rect);
    const auto panel_entity = m_widget_registry.create();
    m_widget_registry.emplace<PanelComponent>(panel_entity, PanelComponent(panel_rect, "Settings"));
    m_widget_registry.emplace<WidgetComponentType>(panel_entity, WidgetComponentType::PANEL);
}

Rectangle SettingsScene::initBrightnessSlider(const Rectangle panel_rect)
{
    const float middle_panel_x = panel_rect.x + panel_rect.width / 2.f;
    const float slider_width = panel_rect.width / 3.f;
    const float slider_height = panel_rect.height / 20.f;
    const float slider_x = middle_panel_x - slider_width;
    const float slider_y = panel_rect.y + slider_height * 2.f;
    const Rectangle slider_rect = {slider_x, slider_y, slider_width, slider_height };
    const auto slider_entity = m_widget_registry.create();
    m_widget_registry.emplace<SliderComponent>(slider_entity, 
                                               SliderComponent(slider_rect, -0.8f, 0.f, &brightness_value, "Brightness:"));
    m_widget_registry.emplace<WidgetComponentType>(slider_entity, WidgetComponentType::SLIDER);
    return slider_rect;
}

void SettingsScene::initResolutionDropdownBox(const Rectangle slider_rect)
{
    
    const float dropdown_box_height = slider_rect.height;
    const float dropdown_box_width = slider_rect.width;
    const float dropdown_box_x = slider_rect.x;
    const float dropdown_box_y = slider_rect.y + slider_rect.height;
    const Rectangle dropdown_box_rect = { dropdown_box_x, dropdown_box_y, dropdown_box_width, dropdown_box_height };
    const char *label_text = "Resolution:";
    const Rectangle label_rect = { dropdown_box_x - MeasureText(label_text, 27), dropdown_box_y, static_cast<float>(MeasureText(label_text, 28)), 28.f};

    for (size_t i = 0; i < resolution_vector.size(); ++i) {
        if (i == resolution_vector.size() - 1) {
            std::string res = TextFormat("%ix%i", static_cast<int>(resolution_vector[i].x), static_cast<int>(resolution_vector[i].y));
            *resolution_list += res;
        } else {
            std::string res = TextFormat("%ix%i;", static_cast<int>(resolution_vector[i].x), static_cast<int>(resolution_vector[i].y));
            *resolution_list += res;
        }
    }
    const auto dropdown_box_entity = m_widget_registry.create();
     
    m_widget_registry.emplace<DropdownBoxComponent>(dropdown_box_entity, 
                                               DropdownBoxComponent(dropdown_box_rect, resolution_list->data(), &choosen_resolution));
    m_widget_registry.emplace<WidgetComponentType>(dropdown_box_entity, WidgetComponentType::DROPDOWNBOX);

    const auto label_entity = m_widget_registry.create();
    m_widget_registry.emplace<LabelComponent>(label_entity, LabelComponent(label_rect, label_text));
    m_widget_registry.emplace<WidgetComponentType>(label_entity, WidgetComponentType::LABEL);
}

void SettingsScene::initButtons(const Rectangle panel_rect)
{
    // Init save button
    const float save_button_width = panel_rect.width / 4.f;
    const float save_button_height = panel_rect.height / 10.f;
    const float save_button_x = panel_rect.x + save_button_width / 1.5f;
    const float save_button_y = panel_rect.y + panel_rect.height - save_button_height * 1.5f;
    const Rectangle save_button_rect = { save_button_x, save_button_y, save_button_width, save_button_height };
    WidgetCallbackComponent::cb save_button_callback = entt::connect_arg<saveValueCallback>;
    WidgetCallbackComponent::cb back_button_callback = entt::connect_arg<backToMainMenuCallback>;

    const auto save_button_entity = m_widget_registry.create();
    m_widget_registry.emplace<ButtonComponent>(save_button_entity, 
                                                    ButtonComponent(save_button_rect, "Save"));
    m_widget_registry.emplace<WidgetComponentType>(save_button_entity, WidgetComponentType::BUTTON);
    m_widget_registry.emplace<WidgetCallbackComponent>(save_button_entity, save_button_callback, this);
    // Init back to menu button 
    const float back_button_x = save_button_x + save_button_width + save_button_width / 2.f;
    const Rectangle back_button_rect = {back_button_x, save_button_y, save_button_width, save_button_height };

    const auto back_button_entity = m_widget_registry.create();
    m_widget_registry.emplace<ButtonComponent>(back_button_entity, 
                                                    ButtonComponent(back_button_rect, "Back"));
    m_widget_registry.emplace<WidgetComponentType>(back_button_entity, WidgetComponentType::BUTTON);
    m_widget_registry.emplace<WidgetCallbackComponent>(back_button_entity, back_button_callback, this);
}

void SettingsScene::saveValueCallback(entt::any data)
{
    SettingsScene *scene = entt::any_cast<SettingsScene*>(data);
    Graphics::setVideoMode(scene->choosen_resolution);
    // Redraw scene
    scene->emmiter.publish(Scene::Messages::SETTINGS);
}

void SettingsScene::backToMainMenuCallback(entt::any data)
{
    SettingsScene *scene = entt::any_cast<SettingsScene*>(data);
    scene->emmiter.publish(Scene::Messages::MAIN_MENU);
}

void SettingsScene::proccessEvents() 
{
    WidgetSystem::proccessEvents(m_widget_registry);
    Game::Instance()->setBrightness(brightness_value);
}

void SettingsScene::update() 
{
    WidgetSystem::update(m_widget_registry);
}

void SettingsScene::draw() const 
{
    BackgroundSystem::draw(m_widget_registry);
    WidgetSystem::draw(m_widget_registry);
}
