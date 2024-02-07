#include "scout_weapon.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
#include "object_component.hpp"
#include "projectile_entity.hpp"
#include "timer_component.hpp"

namespace ScoutWeapon {
    void animationCallback(entt::registry &registry, const entt::entity entity)
    {
        const int idle_tag_id = 0;
        auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
        auto &state = registry.get<WeaponState>(entity);
        sprite.tag.currentFrame = 1;
        sprite.current_tag_id = idle_tag_id; 
        sprite.tag =  ResourceSystem::getAsepriteTag("scout_weapon", idle_tag_id);
        state = WeaponState::IDLE;
    }
}

entt::entity ScoutWeapon::create(entt::registry &registry, const Rectangle rect)
{
    const entt::entity weapon_entity = registry.create();
    const int initial_tag_idle = 0;
    const int firing_tag_id = 1;
    const int last_firing_frame = 4;
    const int timer_id = 1;
    const float fire_cooldown = 0.6;
    const std::string_view key = "scout_weapon";

    auto &hitbox_container = registry.emplace<HitboxComponent::Container>(weapon_entity, weapon_entity);
    auto &timer_container = registry.emplace<TimerComponent::Container>(weapon_entity);

    registry.emplace<Rectangle>(weapon_entity, rect);
    registry.emplace<WeaponState>(weapon_entity, WeaponState::IDLE);
    registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);

    GraphicsComponent::addAnimationComponent(registry, weapon_entity, key, initial_tag_idle, rect, GraphicsComponent::RenderPriority::HIGH);
    GraphicsComponent::addCallback(registry, weapon_entity, firing_tag_id, last_firing_frame, animationCallback);
    HitboxComponent::loadHitboxesInContainer(hitbox_container, key, rect);
    TimerComponent::createFinishedTimerInContainer(timer_container, fire_cooldown, timer_id);
    return weapon_entity;
}

void ScoutWeapon::fire(entt::registry &registry, const entt::entity entity)
{
    const int weapon_reload_timer = 1;
    const int firing_tag = 1;
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
        sprite.tag = ResourceSystem::getAsepriteTag("scout_weapon", firing_tag);
        TimerComponent::reset(timer_container, weapon_reload_timer);
    }
}
