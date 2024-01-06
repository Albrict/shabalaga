#include "behavior_system.hpp"
#include "behavior_component.hpp"
#include "fighter_weapon_entity.hpp"
#include "graphics.hpp"
#include "object_component.hpp"
#include "ship_components.hpp"
#include "timer_component.hpp"

namespace BehaviorSystem {
    void proccessFighterBehavior(entt::registry &registry, const entt::entity entity)
    {
        const auto weapon = registry.get<ShipComponents::Container>(entity).weapon;
        const int direction_timer = 1;
        auto &timer_container = registry.get<TimerComponent::Container>(entity);
        if (TimerComponent::isDone(timer_container, direction_timer)) {
            const Vector2 resolution = Graphics::getCurrentResolution();
            const Rectangle position = registry.get<Rectangle>(entity); 
            auto &velocity = registry.get<VelocityComponent>(entity);
            if (position.x <= resolution.x / 2.f)
                velocity.velocity.x += 300.f;
            else
                velocity.velocity.x -= 300.f;
            TimerComponent::reset(timer_container, direction_timer);
        }
        FighterWeaponEntity::fire(registry, weapon);
    }
}
void BehaviorSystem::proccessEvents(entt::registry &registry)
{
    auto view = registry.view<BehaviorComponent::Type>();
    for (auto [entity, type] : view.each()) {
        switch(type) {
        case BehaviorComponent::Type::FIGHTER:
            proccessFighterBehavior(registry, entity);
            break;
        }
    }
}

void BehaviorSystem::update(entt::registry &registry)
{

}
