#include "fuel_bar.hpp"
#include "graphics_component.hpp"
#include "object_component.hpp"
#include "timer_component.hpp"

void FuelBar::create(entt::registry &registry, const Rectangle rect)
{
    const entt::entity fuel_bar = registry.create();
    const std::string_view key = "fuel_bar";
    const float decrease_fuel_time = 10.f;
    const int timer_id = 1;
    TimerComponent::Container &container = registry.emplace<TimerComponent::Container>(fuel_bar);

    registry.emplace<Rectangle>(fuel_bar, rect);
    registry.emplace<ObjectType>(fuel_bar, ObjectType::HUD); 
    GraphicsComponent::addSpriteComponent(registry, fuel_bar, key, rect, GraphicsComponent::RenderPriority::NONE);
    TimerComponent::createTimerInContainer(container, decrease_fuel_time, timer_id);
}
