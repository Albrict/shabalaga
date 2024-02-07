#include "fighter_weapon_entity.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
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

entt::entity FighterWeaponEntity::create(entt::registry &object_registry, const Rectangle rect, const float fire_cooldown)
{
    const entt::entity weapon_entity = object_registry.create();
    const int tag_id = 0;
    const int last_firing_frame = 3;
    const std::string_view key = "fighter_weapon";
    const int timer_id = 1;
    auto &hitbox_container = object_registry.emplace<HitboxComponent::Container>(weapon_entity, weapon_entity);
    auto &timer_container = object_registry.emplace<TimerComponent::Container>(weapon_entity);

    object_registry.emplace<Rectangle>(weapon_entity, rect);
    object_registry.emplace<WeaponState>(weapon_entity, WeaponState::IDLE);
    object_registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);

    GraphicsComponent::addAnimationComponent(object_registry, weapon_entity, key, 1, rect, GraphicsComponent::RenderPriority::HIGH);
    GraphicsComponent::addCallback(object_registry, weapon_entity, tag_id, last_firing_frame, animationCallback);
    HitboxComponent::loadHitboxesInContainer(hitbox_container, key, rect);
    TimerComponent::createFinishedTimerInContainer(timer_container, fire_cooldown, timer_id);
    return weapon_entity;
}

void FighterWeaponEntity::fire(entt::registry &registry, const entt::entity entity)
{
    const int weapon_reload_timer = 1;
    auto &state = registry.get<WeaponState>(entity);
    auto &timer_container = registry.get<TimerComponent::Container>(entity); 
    if (state == WeaponState::IDLE && TimerComponent::isDone(timer_container, weapon_reload_timer)) {
        auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
        const auto &hitboxes = registry.get<HitboxComponent::Container>(entity).hitboxes; // get hitboxes from container
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
