#include "fighter_weapon_entity.hpp"
#include "collision_component.hpp"
#include "graphics_component.hpp"
#include "object_component.hpp"
#include "projectile_entity.hpp"
#include "resource_system.hpp"
#include "timer_component.hpp"

namespace FighterWeaponEntity {
    void animationCallback(entt::registry &registry, const entt::entity entity)
    {
        auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
        auto &state = registry.get<WeaponState>(entity);
        sprite.tag.currentFrame = 1;
        sprite.current_tag_id = 1; 
        sprite.tag =  ResourceSystem::getAsepriteTag("fighter_weapon", 1);
        state = WeaponState::IDLE;
    }
}

void FighterWeaponEntity::fire(entt::registry &registry, const entt::entity entity)
{
    const int weapon_reload_timer = 1;
    auto &state = registry.get<WeaponState>(entity);
    auto &timer_container = registry.get<TimerComponent::Container>(entity); 
    if (state == WeaponState::IDLE && TimerComponent::isDone(timer_container, weapon_reload_timer)) {
        auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
        const auto &hitboxes = registry.get<Collision::DynamicComponent>(entity).hitboxes; // get hitboxes from container
        for (const auto hitbox : hitboxes) {
            Rectangle rect = hitbox.rect;
            const Vector2 position = {rect.x - rect.width / 2.f, rect.y};
            rect.width *= 1.1f;
            rect.height *= 1.1f;
            ProjectileEntity::create(registry, rect, ProjectileEntity::Type::BULLET_PROJECTILE);
        }
        state = WeaponState::FIRING;
        sprite.current_tag_id = 0;
        sprite.tag = ResourceSystem::getAsepriteTag("fighter_weapon", 0);
        TimerComponent::reset(timer_container, weapon_reload_timer);
    }
}
