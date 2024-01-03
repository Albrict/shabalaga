#include "dropdown_box_system.hpp"
#include "widget_components.hpp"
#include "../include/raygui.h"

using WidgetComponents::DropdownBox;

void DropdownBoxSystem::proccessEvents(entt::registry &registry)
{
    auto view = registry.view<DropdownBox>();
    for (auto [entity, dropdown_box] : view.each()) {
        if (GuiDropdownBox(dropdown_box.rect, dropdown_box.text, dropdown_box.active, dropdown_box.edit_mode))
            dropdown_box.edit_mode = !dropdown_box.edit_mode;
    }
}

void DropdownBoxSystem::update(entt::registry &registry)
{

}

void DropdownBoxSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<DropdownBox>();
    for (auto [entity, dropdown_box] : view.each()) 
        GuiDropdownBox(dropdown_box.rect, dropdown_box.text, dropdown_box.active, dropdown_box.edit_mode);
}
