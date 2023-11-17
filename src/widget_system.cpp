#include "dropdown_box_system.hpp"
#include "widget_components.hpp"
#include "widget_system.hpp"
#include "button_system.hpp"
#include "slider_system.hpp"
#include "panel_system.hpp"
#include "label_system.hpp"

void WidgetSystem::proccessEvents(entt::registry &registry)
{
    auto view = registry.view<const WidgetComponentType>();   
    for (auto [entity, type] : view.each()) {
        switch(type) {
        using enum WidgetComponentType;
        case BUTTON:
            ButtonSystem::proccessEvents(registry);
            break;
        case SLIDER:
            SliderSystem::proccessEvents(registry);
            break; 
        case DROPDOWNBOX:
            DropdownBoxSystem::proccessEvents(registry);
            break;
        case PANEL:
            PanelSystem::proccessEvents(registry);
            break;
        default:
            break;
        }
    }
}

void WidgetSystem::update(entt::registry &registry)
{
    auto view = registry.view<const WidgetComponentType>();   
    for (auto [entity, type] : view.each()) {
        switch(type) {
        using enum WidgetComponentType;
        case BUTTON:
            ButtonSystem::update(registry);
            break;
        case SLIDER:
            SliderSystem::update(registry);
            break; 
        case DROPDOWNBOX:
            DropdownBoxSystem::update(registry);
            break;
        case PANEL:
            PanelSystem::update(registry);
            break;
        default:
            break;
        }
    }
}

void WidgetSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<const WidgetComponentType>();   
    for (auto [entity, type] : view.each()) {
        switch(type) {
        using enum WidgetComponentType;
        case BUTTON:
            ButtonSystem::draw(registry);
            break;
        case SLIDER:
            SliderSystem::draw(registry);
            break; 
        case DROPDOWNBOX:
            DropdownBoxSystem::draw(registry);
            break;
        case LABEL:
            LabelSystem::draw(registry);
            break;
        case PANEL:
            PanelSystem::draw(registry);
            break;
        }
    }
}
