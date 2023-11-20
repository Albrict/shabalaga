#include "label_system.hpp"
#include "widget_components.hpp"
#include "../include/raygui.h"

void LabelSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<const LabelComponent>();
    for (const auto [entity, label] : view.each())
        GuiLabel(label.rect, label.text);
}
