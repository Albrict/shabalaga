#include "fighter_weapon_entity.hpp"
#include "bullet_entity.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
#include "object_component.hpp"
#include "timer_component.hpp"

namespace FighterWeaponEntity {
    void animationCallback(entt::registry &registry, const entt::entity entity)
    {
        auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
        auto &state = registry.get<WeaponState>(entity);
        sprite.sprite->setTagFrame(1);
        sprite.sprite->loadAsepriteTag("idle");
        state = WeaponState::IDLE;
    }
}

entt::entity FighterWeaponEntity::create(entt::registry &object_registry, const Rectangle rect)
{
    const int tag_id = 0;
    const int last_firing_frame = 3;
    const auto weapon_entity = object_registry.create();
    auto &sprite = object_registry.emplace<GraphicsComponent::Animation>(weapon_entity);
    auto &hitbox_container = object_registry.emplace<HitboxComponent::Container>(weapon_entity, weapon_entity);
    auto &timer_container = object_registry.emplace<TimerComponent::Container>(weapon_entity);

    object_registry.emplace<Rectangle>(weapon_entity, rect);
    object_registry.emplace<WeaponState>(weapon_entity, WeaponState::IDLE);

    object_registry.emplace<GraphicsComponent::RenderPriority>(weapon_entity, GraphicsComponent::RenderPriority::HIGH);
    object_registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);
    object_registry.emplace<GraphicsComponent::RenderType>(weapon_entity, GraphicsComponent::RenderType::ANIMATION);

    sprite = GraphicsComponent::createAnimation("fighter_weapon", "idle", rect.width, rect.height);
    GraphicsComponent::addCallback(sprite, tag_id, last_firing_frame, animationCallback);
    HitboxComponent::createHitboxInContainerFromSprite(object_registry, hitbox_container, sprite, "left_gun", {rect.x, rect.y});
    HitboxComponent::createHitboxInContainerFromSprite(object_registry, hitbox_container, sprite, "right_gun", {rect.x, rect.y});
    TimerComponent::createTimerInContainer(timer_container, 1.0f, 1);
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
            rect.width = 16;
            rect.height = 16;
            BulletEntity::create(registry, rect);
        }
        state = WeaponState::FIRING;
        sprite.sprite->loadAsepriteTag("firing");
        TimerComponent::reset(timer_container, weapon_reload_timer);
    }
}
