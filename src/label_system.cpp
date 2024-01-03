#include "label_system.hpp"
#include "widget_components.hpp"
#include "../include/raygui.h"

using WidgetComponents::Label;

void LabelSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<const Label>();
    for (const auto [entity, label] : view.each())
        GuiLabel(label.rect, label.text);
}
