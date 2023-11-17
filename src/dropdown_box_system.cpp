#include "dropdown_box_system.hpp"
#include "widget_components.hpp"
#include "../include/raygui.h"

void DropdownBoxSystem::proccessEvents(entt::registry &registry)
{
    auto view = registry.view<DropdownBoxComponent>();
    for (auto [entity, dropdown_box] : view.each()) {
        if (GuiDropdownBox(dropdown_box.rect, dropdown_box.text, dropdown_box.active, dropdown_box.edit_mode)){
            dropdown_box.edit_mode = !dropdown_box.edit_mode;
            printf("Choosen result:%d\n", *dropdown_box.active);
        }
    }
}

void DropdownBoxSystem::update(entt::registry &registry)
{

}

void DropdownBoxSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<DropdownBoxComponent>();
    for (auto [entity, dropdown_box] : view.each()) 
        GuiDropdownBox(dropdown_box.rect, dropdown_box.text, dropdown_box.active, dropdown_box.edit_mode);
}
