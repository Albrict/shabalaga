#include "weapon_entity.hpp"
#include "auto_cannon_projectile_entity.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
#include "input_component.hpp"
#include "object_component.hpp"
#include "timer_component.hpp"

namespace WeaponEntity {
    namespace AutoCannon {
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
    
        void animationCallback(entt::registry &registry, const entt::entity entity)
        {
            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
            auto &state = registry.get<WeaponState>(entity);
            sprite.tag.currentFrame = 1;
            sprite.current_tag_id = 0;
            sprite.tag = ResourceSystem::getAsepriteTag("auto_cannon", 0);
            state = WeaponState::IDLE;
        }

        void create(entt::registry &object_registry, const Rectangle rect, const entt::entity weapon_entity)
        {
                const int firing_tag_id = 1;
                const int firing_last_frame = 5;
                const std::string_view key = "auto_cannon";

                auto &container = object_registry.emplace<HitboxComponent::Container>(weapon_entity, weapon_entity);
                auto &input_container = object_registry.emplace<InputComponent::Container>(weapon_entity);

                object_registry.emplace<Rectangle>(weapon_entity, rect);
                object_registry.emplace<WeaponState>(weapon_entity, WeaponState::IDLE);
                object_registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);
                
                InputComponent::create(input_container, fireAutoCannon, KEY_SPACE, InputComponent::Type::DOWN);
                GraphicsComponent::addAnimationComponent(object_registry, weapon_entity, key, 0, rect, GraphicsComponent::RenderPriority::HIGH);
                GraphicsComponent::addCallback(object_registry, weapon_entity, firing_tag_id, firing_last_frame, animationCallback);
                HitboxComponent::loadHitboxesInContainer(container, "auto_cannon", rect);
        }
    }
    namespace FighterWeapon {
        void animationCallback(entt::registry &registry, const entt::entity entity)
        {
            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
            auto &state = registry.get<WeaponState>(entity);
            sprite.tag.currentFrame = 1;
            sprite.current_tag_id = 1; 
            sprite.tag =  ResourceSystem::getAsepriteTag("fighter_weapon", 1);
            state = WeaponState::IDLE;
        }

        entt::entity create(entt::registry &object_registry, const Rectangle rect, const float fire_cooldown)
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
    }

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

        entt::entity create(entt::registry &registry, const Rectangle rect)
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
    }

    namespace BigSpaceGun {
        void initAnimationCallback(entt::registry &registry, const entt::entity entity)
        {
            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
            auto &state = registry.get<WeaponState>(entity);
            sprite.tag.currentFrame = 1;
            sprite.current_tag_id = 2; 
            sprite.tag =  ResourceSystem::getAsepriteTag("big_space_gun", 2);
            state = WeaponState::IDLE;
        }

        void create(entt::registry &registry, const Rectangle rect, const entt::entity weapon_entity)
        {
            const int init_tag = 1;
            const int last_init_frame = 10;
            const std::string_view key = "big_space_gun";

            auto &container = registry.emplace<HitboxComponent::Container>(weapon_entity, weapon_entity);
            auto &input_container = registry.emplace<InputComponent::Container>(weapon_entity);

            registry.emplace<Rectangle>(weapon_entity, rect);
            registry.emplace<WeaponState>(weapon_entity, WeaponState::INIT);
            registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);
            
//            InputComponent::create(input_container, fireAutoCannon, KEY_SPACE, InputComponent::Type::DOWN);
            GraphicsComponent::addAnimationComponent(registry, weapon_entity, key, init_tag, rect, GraphicsComponent::RenderPriority::HIGH);
            GraphicsComponent::addCallback(registry, weapon_entity, init_tag, last_init_frame, initAnimationCallback);
            HitboxComponent::loadHitboxesInContainer(container, key, rect);
        }
    }

    namespace Zapper {
        void initAnimationCallback(entt::registry &registry, const entt::entity entity)
        {
            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
            auto &state = registry.get<WeaponState>(entity);
            sprite.tag.currentFrame = 1;
            sprite.current_tag_id = 2; 
            sprite.tag =  ResourceSystem::getAsepriteTag("zapper", 0);
            state = WeaponState::IDLE;
        }

        void create(entt::registry &registry, const Rectangle rect, const entt::entity weapon_entity)
        {
            const int init_tag = 2;
            const int last_init_frame = 13;
            const std::string_view key = "zapper";

            auto &container = registry.emplace<HitboxComponent::Container>(weapon_entity, weapon_entity);
            auto &input_container = registry.emplace<InputComponent::Container>(weapon_entity);

            registry.emplace<Rectangle>(weapon_entity, rect);
            registry.emplace<WeaponState>(weapon_entity, WeaponState::INIT);
            registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);
            
//            InputComponent::create(input_container, fireAutoCannon, KEY_SPACE, InputComponent::Type::DOWN);
            GraphicsComponent::addAnimationComponent(registry, weapon_entity, key, init_tag, rect, GraphicsComponent::RenderPriority::HIGH);
            GraphicsComponent::addCallback(registry, weapon_entity, init_tag, last_init_frame, initAnimationCallback);
            HitboxComponent::loadHitboxesInContainer(container, key, rect);
        }
    }
}

entt::entity WeaponEntity::create(entt::registry &registry, const Rectangle rect, const PlayerType type)
{
    const entt::entity weapon = registry.create();
    registry.emplace<PlayerType>(weapon, type);
    switch(type) {
    case AUTO_CANNON:
        AutoCannon::create(registry, rect, weapon);
        break;
    case ZAPPER:
        Zapper::create(registry, rect, weapon);
        break;
    case BIG_SPACE_GUN:
        BigSpaceGun::create(registry, rect, weapon);
        break;
    }
    return weapon;
}


entt::entity WeaponEntity::create(entt::registry &registry, const Rectangle rect, const EnemyType type, const float cooldown)
{
    switch(type) {
    case FIGHTER_WEAPON:
        return FighterWeapon::create(registry, rect, cooldown);
        break;
    case SCOUT_WEAPON:
        return ScoutWeapon::create(registry, rect);
        break;
    }
}
