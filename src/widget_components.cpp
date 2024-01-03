#include "widget_components.hpp"
#include "graphics_component.hpp"
#include "object_component.hpp"

entt::entity WidgetComponents::createButton(entt::registry &object_registry, const Rectangle rect, const char *text)
{
    auto entity = object_registry.create();
    object_registry.emplace<Button>(entity, rect, text);
    object_registry.emplace<Type>(entity, Type::BUTTON);
    object_registry.emplace<GraphicsComponent::RenderType>(entity, GraphicsComponent::RenderType::WIDGET);
    object_registry.emplace<ObjectType>(entity, ObjectType::WIDGET);
    return entity;
}

entt::entity WidgetComponents::createPanel(entt::registry &object_registry, const Rectangle rect, const char *text)
{
    auto panel_entity = object_registry.create();
    object_registry.emplace<Panel>(panel_entity, rect, text);
    object_registry.emplace<Type>(panel_entity, Type::PANEL);
    object_registry.emplace<GraphicsComponent::RenderType>(panel_entity, GraphicsComponent::RenderType::WIDGET);
    object_registry.emplace<ObjectType>(panel_entity, ObjectType::WIDGET);
    return panel_entity;
}


entt::entity WidgetComponents::createSlider(entt::registry &object_registry, const Rectangle rect, 
                            const float min_value, const float max_value, float *value, const char *text_left, const char *text_right)
{
    auto slider_entity = object_registry.create();
    auto &slider = object_registry.emplace<Slider>(slider_entity);
    object_registry.emplace<Type>(slider_entity, Type::SLIDER);
    object_registry.emplace<ObjectType>(slider_entity, ObjectType::WIDGET);
    object_registry.emplace<GraphicsComponent::RenderType>(slider_entity, GraphicsComponent::RenderType::WIDGET);

    slider.max_value = max_value;
    slider.min_value = min_value;
    slider.value = value;
    slider.rect = rect;
    slider.text_left = text_left;
    slider.text_right = text_right;

    return slider_entity;
}

entt::entity WidgetComponents::createDropDownBox(entt::registry &object_registry, const Rectangle rect, const char *text, int *active)
{
    auto dropdown_box_entity = object_registry.create();
    object_registry.emplace<DropdownBox>(dropdown_box_entity, rect, text, active, false);
    object_registry.emplace<Type>(dropdown_box_entity, Type::DROPDOWNBOX);
    object_registry.emplace<ObjectType>(dropdown_box_entity, ObjectType::WIDGET);
    object_registry.emplace<GraphicsComponent::RenderType>(dropdown_box_entity, GraphicsComponent::RenderType::WIDGET);
    return dropdown_box_entity;
}


entt::entity WidgetComponents::createLabel(entt::registry &object_registry, const Rectangle rect, const char *text)
{
    auto label_entity = object_registry.create();
    object_registry.emplace<Label>(label_entity, rect, text);
    object_registry.emplace<Type>(label_entity, Type::LABEL);
    object_registry.emplace<GraphicsComponent::RenderType>(label_entity, GraphicsComponent::RenderType::WIDGET);
    return label_entity;
}
