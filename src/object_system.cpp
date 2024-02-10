#include "object_system.hpp"
#include "behavior_system.hpp"
#include "collision_system.hpp"
#include "fuel_system.hpp"
#include "object_component.hpp"
#include "ship_components_system.hpp"
#include "widget_system.hpp"
#include "game_master_system.hpp"
#include "timer_system.hpp"

void ObjectSystem::proccessEvents(entt::registry &registry)
{
    auto view = registry.view<ObjectType>();
    for (auto [entity, type] : view.each()) {
        switch(type) {
        using enum ObjectType;
        case WIDGET:
            WidgetSystem::proccessEvents(registry);
            break;
        case GAME_MASTER:
            GameMasterSystem::proccessEvents(registry, entity);            
            break;
        case HUD:
            FuelSystem::proccessEvents(registry, entity);
            break;
        default:
            break;
        }
    }
    BehaviorSystem::proccessEvents(registry);
}

void ObjectSystem::update(entt::registry &registry)
{
    auto view = registry.view<ObjectType>();
    for (auto [entity, type] : view.each()) {
        switch(type) {
        using enum ObjectType;
            break;
        case GAME_MASTER:
            GameMasterSystem::update(registry, entity);
            break;
        case WIDGET:
            WidgetSystem::update(registry);
            break;
        default:
            break;
        }
    }
    TimerSystem::update(registry);
    BehaviorSystem::update(registry);
    ShipComponentsSystem::update(registry);
    CollisionSystem::updateHitboxes(registry);
}
