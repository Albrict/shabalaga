#include "slider_system.hpp"
#include "widget_components.hpp"
#include "../include/raygui.h"

void SliderSystem::proccessEvents(const entt::registry &registry)
{
    auto view = registry.view<const SliderComponent>();
    for (auto [entity, slider_component] : view.each()) 
        GuiSlider(slider_component.rect, slider_component.text_left, slider_component.text_right,
                  slider_component.value, slider_component.min_value, slider_component.max_value);
}

void SliderSystem::update(entt::registry &registry)
{

}

void SliderSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<const SliderComponent>();
    for (const auto [entity, slider_component] : view.each())
        GuiSlider(slider_component.rect, slider_component.text_left, slider_component.text_right,
                  slider_component.value, slider_component.min_value, slider_component.max_value);
}
