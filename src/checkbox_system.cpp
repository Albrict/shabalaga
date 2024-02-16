#include "checkbox_system.hpp"
#include "widget_components.hpp"
#include "../include/raygui.h"

void CheckBoxSystem::draw(const entt::registry &registry)
{
    const auto &view = registry.view<WidgetComponents::CheckBox>();    
    for (auto [entity, checkbox] : view.each())
        GuiCheckBox(checkbox.rect, checkbox.text, checkbox.checked);
}
