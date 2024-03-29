#include "widget_components.hpp"
#include "panel_system.hpp"
#include "widget_system.hpp"
#include "../include/raygui.h"

using WidgetComponents::Panel;

void PanelSystem::proccessEvents(const entt::registry &registry)
{
    ;
}

void PanelSystem::update(entt::registry &registry)
{
    ;
}

void PanelSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<const Panel>();
    for (auto [entity, panel_component] : view.each())
        GuiPanel(panel_component.rect, panel_component.text);
}
