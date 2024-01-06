#include "button_system.hpp"
#include "resource_system.hpp"
#include "widget_components.hpp"
#include "../include/raygui.h"

using WidgetComponents::Button;
using WidgetComponents::WidgetCallback;

void ButtonSystem::proccessEvents(const entt::registry &registry)
{
    auto view = registry.view<const Button, const WidgetCallback>();
    for (auto [entity, button_component, widget_callback] : view.each()) {
        if (GuiButton(button_component.rect, button_component.text)) {
            const int sound = GetRandomValue(0, 4);
            PlaySound(ResourceSystem::getSound("ui_01")); 
            if (widget_callback.callback) 
                widget_callback.callback(widget_callback.data);
        }
    }
}

void ButtonSystem::update(entt::registry &registry)
{
    ;
}

void ButtonSystem::draw(const entt::registry &registry) 
{
    auto view = registry.view<const Button>();
    for (const auto [entity, button_component] : view.each())
        GuiButton(button_component.rect, button_component.text);
}
