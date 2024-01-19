#include "behavior_system.hpp"
#include "behavior_component.hpp"
#include "bomber_entity.hpp"
#include "fighter_weapon_entity.hpp"
#include "graphics.hpp"
#include "object_component.hpp"
#include "scout_weapon.hpp"
#include "ship_components.hpp"
#include "timer_component.hpp"

namespace BehaviorSystem {
    void proccessFighterBehavior(entt::registry &registry, const entt::entity entity)
    {
        const auto weapon = registry.get<ShipComponents::Container>(entity).weapon;
        const int direction_timer = 1;
        auto &timer_container = registry.get<TimerComponent::Container>(entity);
        FighterWeaponEntity::fire(registry, weapon);
        if (TimerComponent::isDone(timer_container, direction_timer)) {
            const Vector2 resolution = Graphics::getCurrentResolution();
            const Rectangle position = registry.get<Rectangle>(entity); 
            auto &velocity = registry.get<VelocityComponent>(entity);
            const int chance = GetRandomValue(1, 10);
            if (chance >= 6) {
                if (position.x <= resolution.x / 2.f)
                    velocity.velocity.x += 300.f;
                else
                    velocity.velocity.x -= 300.f;
            }
            TimerComponent::reset(timer_container, direction_timer);
        }
    }

    void proccessBomberBehavior(entt::registry &registry, const entt::entity entity)
    {
        const Rectangle player_position = registry.get<ObjectComponent::PlayerPosition>(entity).rect;
        const Rectangle bomber_position = registry.get<Rectangle>(entity);
        if (CheckCollisionRecs(player_position, bomber_position))
            BomberEntity::destroy(registry, entity);
    }

    void proccessScoutBehavior(entt::registry &registry, const entt::entity entity)
    {
        const auto weapon = registry.get<ShipComponents::Container>(entity).weapon;
        const auto rect = registry.get<Rectangle>(entity);
        const int direction_timer = 1;
        const int collision_timer = 2;
        const Vector2 resolution = Graphics::getCurrentResolution();
        ScoutWeapon::fire(registry, weapon);
        auto &timer_container = registry.get<TimerComponent::Container>(entity);
        auto &velocity = registry.get<VelocityComponent>(entity);
        

        if (TimerComponent::isDone(timer_container, direction_timer)) {
            const Vector2 resolution = Graphics::getCurrentResolution();
            const float velocity_x = resolution.x / 4.f;
            if (rect.y + rect.height >= resolution.y - resolution.y / 4.f)
                velocity.velocity.y *= -1.f;
            else if (rect.y <= 0.f)
                velocity.velocity.y *= -1.f;
            if (rect.x <= 0.f) {
                velocity.velocity.x *= -1.f;
            } else if (rect.x + rect.width >= resolution.x) {
                velocity.velocity.x *= -1.f;
            } else {
                const int direction = GetRandomValue(-1, 1);
                velocity.velocity.x = velocity_x;
                velocity.velocity.x *= direction;
            }
            TimerComponent::reset(timer_container, direction_timer);
        }
    }
}
void BehaviorSystem::proccessEvents(entt::registry &registry)
{
    auto view = registry.view<BehaviorComponent::Type>();
    for (auto [entity, type] : view.each()) {
        switch(type) {
        using enum BehaviorComponent::Type;
        case BehaviorComponent::Type::FIGHTER:
            proccessFighterBehavior(registry, entity);
            break;
        case BehaviorComponent::Type::BOMBER:
            proccessBomberBehavior(registry, entity);
            break;
        case BehaviorComponent::Type::SCOUT:
            proccessScoutBehavior(registry, entity);
            break;
        }
    }
}

void BehaviorSystem::update(entt::registry &registry)
{

}
