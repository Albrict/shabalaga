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
    for (const auto [entity, button_component] : view.each()) {
        if (button_component.icon != -1)
            GuiButton(button_component.rect, GuiIconText(button_component.icon, button_component.text));
        else
            GuiButton(button_component.rect, button_component.text);
    }
}
