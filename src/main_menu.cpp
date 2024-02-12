#include "main_menu.hpp"
#include "fade_component.hpp"
#include "graphics.hpp"
#include "resource_system.hpp"
#include "widget_components.hpp"
#include "object_system.hpp"
#include "background_component.hpp"
#include "graphics_system.hpp"


MainMenuScene::MainMenuScene()
{
    using WidgetComponents::WidgetCallback;
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
        if (i == 0)
            object_registry.emplace<WidgetCallback>(entity, button_callback[i], this);
        else
            object_registry.emplace<WidgetCallback>(entity, button_callback[i], nullptr);
        initial_y += button_height * 2;
    }
    BackgroundComponent::create(object_registry, ResourceSystem::getTexture("menu_background"), -30.f);

    fade_in = object_registry.create();
    fade_out = object_registry.create();

    object_registry.emplace<FadeEffect::Component>(fade_in, FadeEffect::create(0.4f, FadeEffect::Type::FADE_OUT));
    object_registry.emplace<FadeEffect::Component>(fade_out, FadeEffect::create(0.4f, FadeEffect::Type::FADE_IN, true));
}

void MainMenuScene::proccessEvents()
{
    if (FadeEffect::isDone(object_registry.get<FadeEffect::Component>(fade_out))) {
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::HANGAR,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }
    ObjectSystem::proccessEvents(object_registry);
}

void MainMenuScene::update()
{
    GraphicsSystem::update(object_registry);
    ObjectSystem::update(object_registry);
}

void MainMenuScene::draw() const
{
    Graphics::beginRender();
        GraphicsSystem::draw(object_registry);
    Graphics::endRender();
}
