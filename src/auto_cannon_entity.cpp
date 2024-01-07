#include "auto_cannon_entity.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
#include "input_component.hpp"
#include "object_component.hpp"
#include "auto_cannon_projectile_entity.hpp"

namespace AutoCannonEntity {
    void fireAutoCannon(entt::registry &registry, const entt::entity entity)
    {
        auto &state = registry.get<WeaponState>(entity);
        auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
                 
        if (state == WeaponState::IDLE) {
            const auto hitboxes = registry.get<HitboxComponent::Container>(entity).hitboxes; // get hitboxes from container
            const int sound = GetRandomValue(0, 1);
            const std::string_view key = "auto_cannon";
            for (const auto hitbox : hitboxes) {
                Rectangle rect = hitbox.rect;
                rect.x -= rect.width / 2.f;
                rect.width *= 2;
                rect.height *= 2;
                AutoCannonProjectileEntity::create(registry, rect);
            }
            state = WeaponState::FIRING;
            sprite.tag = ResourceSystem::getAsepriteTag(key, 1);
            sprite.current_tag_id = 1;

            if (sound > 0) 
                PlaySound(ResourceSystem::getSound("attack_01"));
            else
                PlaySound(ResourceSystem::getSound("attack_02"));
        }
    }
    
    void autoCannonCallback(entt::registry &registry, const entt::entity entity)
    {
        auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
        auto &state = registry.get<WeaponState>(entity);
        sprite.tag.currentFrame = 1;
        sprite.current_tag_id = 0;
        sprite.tag = ResourceSystem::getAsepriteTag("auto_cannon", 0);
        state = WeaponState::IDLE;
    }
}

entt::entity AutoCannonEntity::create(entt::registry &object_registry, const Rectangle rect)
{
        const auto weapon_entity = object_registry.create();
        const int firing_tag_id = 1;
        const int firing_last_frame = 5;

        auto &sprite = object_registry.emplace<GraphicsComponent::Animation>(weapon_entity);
        auto &container = object_registry.emplace<HitboxComponent::Container>(weapon_entity, weapon_entity);
        auto &input_container = object_registry.emplace<InputComponent::Container>(weapon_entity);

        object_registry.emplace<Rectangle>(weapon_entity, rect);
        object_registry.emplace<WeaponState>(weapon_entity, WeaponState::IDLE);
        
        object_registry.emplace<GraphicsComponent::RenderPriority>(weapon_entity, GraphicsComponent::RenderPriority::HIGH);
        object_registry.emplace<GraphicsComponent::RenderType>(weapon_entity, GraphicsComponent::RenderType::ANIMATION);
        object_registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);

        InputComponent::create(input_container, fireAutoCannon, KEY_SPACE, InputComponent::Type::DOWN);

        sprite = GraphicsComponent::createAnimation("auto_cannon", 0, rect.width, rect.height);
        
        GraphicsComponent::addCallback(sprite, firing_tag_id, firing_last_frame, autoCannonCallback);
        HitboxComponent::loadHitboxesInContainer(container, "auto_cannon", rect);

        return weapon_entity;
}
