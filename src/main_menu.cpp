#include "main_menu.hpp"
#include "graphics.hpp"
#include "resource_system.hpp"
#include "widget_components.hpp"
#include "object_system.hpp"
#include "background_component.hpp"
#include "graphics_system.hpp"

using WidgetComponents::WidgetCallback;

MainMenuScene::MainMenuScene()
{
    const Vector2 resolution = Graphics::getCurrentResolution();
    const float button_width = resolution.x / 10.f;
    const float button_height = resolution.y / 20.f;
    const float initial_x = resolution.x / 2 - button_width / 2;
    const std::array<const char*, 3> button_labels = { "Play", "Settings", "Exit" };
    const std::array<WidgetCallback::cb, 3> button_callback {
        {
            playCallback,
            settingsCallback,
            exitCallback
        }
    };
    float initial_y = resolution.y / 3 - button_height;
    for (size_t i = 0; i < button_callback.size(); ++i) {
        const Rectangle button_rect = {initial_x, initial_y, button_width, button_height};
        const auto entity = WidgetComponents::createButton(object_registry, button_rect, button_labels[i]);
        object_registry.emplace<WidgetCallback>(entity, button_callback[i], nullptr);
        initial_y += button_height * 2;
    }
    BackgroundComponent::create(object_registry, ResourceSystem::getTexture("menu_background"));
}

void MainMenuScene::proccessEvents()
{
    ObjectSystem::proccessEvents(object_registry);
}

void MainMenuScene::update()
{
    ObjectSystem::update(object_registry);
}

void MainMenuScene::draw() const
{
    Graphics::beginRender();
        GraphicsSystem::draw(object_registry);
    Graphics::endRender();
}
