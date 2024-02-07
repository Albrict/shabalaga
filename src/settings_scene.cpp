#include "settings_scene.hpp"
#include "background_component.hpp"
#include "graphics.hpp"
#include "graphics_system.hpp"
#include "message_system.hpp"
#include "object_system.hpp"
#include "widget_components.hpp"
#include "resource_system.hpp"

SettingsScene::SettingsScene()
{
    resolution_list = std::make_unique<std::string>();
    resolution_vector = Graphics::getPossibleResolutions();
    choosen_resolution = resolution_vector.size() - 1;
    initWidgets();
    BackgroundComponent::create(object_registry, ResourceSystem::getTexture("menu_background"));
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
//    initResolutionDropdownBox(slider_rect);
    initButtons(panel_rect);
    WidgetComponents::createPanel(object_registry, panel_rect, "Settings");
}

Rectangle SettingsScene::initBrightnessSlider(const Rectangle panel_rect)
{
    const float middle_panel_x = panel_rect.x + panel_rect.width / 2.f;
    const float slider_width = panel_rect.width / 3.f;
    const float slider_height = panel_rect.height / 20.f;
    const float slider_x = middle_panel_x - slider_width;
    const float slider_y = panel_rect.y + slider_height * 2.f;
    const float min_value = -0.8f;
    const float max_value = 0.0f;
    const Rectangle slider_rect = {slider_x, slider_y, slider_width, slider_height };
    WidgetComponents::createSlider(object_registry, slider_rect, min_value, max_value, &brightness_value, "Brightness");
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
    const Rectangle label_rect = { dropdown_box_x - MeasureText(label_text, Graphics::getCurrentFontSize()), dropdown_box_y, 
                                static_cast<float>(MeasureText(label_text, Graphics::getCurrentFontSize())), static_cast<float>(Graphics::getCurrentFontSize())};

    for (size_t i = 0; i < resolution_vector.size(); ++i) {
        if (i == resolution_vector.size() - 1) {
            std::string res = TextFormat("%ix%i", static_cast<int>(resolution_vector[i].x), static_cast<int>(resolution_vector[i].y));
            *resolution_list += res;
        } else {
            std::string res = TextFormat("%ix%i;", static_cast<int>(resolution_vector[i].x), static_cast<int>(resolution_vector[i].y));
            *resolution_list += res;
        }
    }
    WidgetComponents::createDropDownBox(object_registry, dropdown_box_rect, resolution_list->data(), &choosen_resolution);
    WidgetComponents::createLabel(object_registry, label_rect, label_text);
}


void SettingsScene::initButtons(const Rectangle panel_rect)
{
    // Init save button
    using WidgetComponents::WidgetCallback;
    const float save_button_width = panel_rect.width / 4.f;
    const float save_button_height = panel_rect.height / 10.f;
    const float save_button_x = panel_rect.x + save_button_width / 1.5f;
    const float save_button_y = panel_rect.y + panel_rect.height - save_button_height * 1.5f;
    const Rectangle save_button_rect = { save_button_x, save_button_y, save_button_width, save_button_height };

    const auto save_button_entity = WidgetComponents::createButton(object_registry, save_button_rect, "Save");
    object_registry.emplace<WidgetCallback>(save_button_entity, saveValueCallback, this);

    // Init back to menu button 
    const float back_button_x = save_button_x + save_button_width + save_button_width / 2.f;
    const Rectangle back_button_rect = {back_button_x, save_button_y, save_button_width, save_button_height };

    const auto back_button_entity = WidgetComponents::createButton(object_registry, back_button_rect, "Back"); 
    object_registry.emplace<WidgetCallback>(back_button_entity, backToMainMenuCallback, nullptr);
}

void SettingsScene::saveValueCallback(entt::any data)
{
    MessageSystem::Message msg = {
        .msg = MessageSystem::SceneMessage::SETTINGS,
        .type = MessageSystem::Type::SCENE_MESSAGE 
    };

    SettingsScene *scene = entt::any_cast<SettingsScene*>(data);
    Graphics::setVideoMode(scene->choosen_resolution);
    MessageSystem::sendMessage(msg); // Redraw Scene
}

void SettingsScene::backToMainMenuCallback(entt::any data)
{
    MessageSystem::Message msg = {
        .msg = MessageSystem::SceneMessage::MENU,
        .type = MessageSystem::Type::SCENE_MESSAGE
    };
    MessageSystem::sendMessage(msg);
}

void SettingsScene::proccessEvents() 
{
    ObjectSystem::proccessEvents(object_registry);
    Graphics::setBrightness(brightness_value);
}

void SettingsScene::update() 
{
    ObjectSystem::update(object_registry);
}

void SettingsScene::draw() const 
{
    Graphics::beginRender();
        GraphicsSystem::draw(object_registry);
    Graphics::endRender();
}
