#include "behavior_system.hpp"
#include "behavior_component.hpp"
#include "fighter_weapon_entity.hpp"
#include "object_component.hpp"
#include "ship_components.hpp"
#include "timer_component.hpp"

namespace BehaviorSystem {
    void proccessFighterBehavior(entt::registry &registry, const entt::entity entity)
    {
        const auto weapon = registry.get<ShipComponents::Container>(entity).weapon;
        auto &timer_container = registry.get<TimerComponent::Container>(entity);
        if (TimerComponent::isDone(timer_container, 1)) {
            const int change_direction = GetRandomValue(0, 100);
            if (change_direction >= 50) {
                const int direction = GetRandomValue(0, 10);
                auto &velocity = registry.get<VelocityComponent>(entity);
                if (direction >= 5)
                    velocity.velocity.x += 300.f;
                else
                    velocity.velocity.x -= 300.f;
            }
            TimerComponent::reset(timer_container, 1);
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
