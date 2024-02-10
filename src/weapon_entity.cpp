#include "weapon_entity.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "collision_component.hpp"
#include "input_component.hpp"
#include "object_component.hpp"
#include "projectile_entity.hpp"
#include "timer_component.hpp"

namespace WeaponEntity {
    namespace AutoCannon {
        void fireAutoCannon(entt::registry &registry, const entt::entity entity)
        {
            const unsigned int firing_tag = 1;
            const std::string_view key = "auto_cannon";

            auto &state = registry.get<WeaponState>(entity);
            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
                     
            if (state == WeaponState::IDLE) {
                const auto &hitboxes = registry.get<Collision::DynamicComponent>(entity).hitboxes; // get hitboxes from container
                const int sound = GetRandomValue(0, 1);
                const std::string_view key = "auto_cannon";
                for (const auto hitbox : hitboxes) {
                    Rectangle rect = hitbox.rect;
                    rect.x -= rect.width / 2.f;
                    rect.width *= 2;
                    rect.height *= 2;
                    ProjectileEntity::create(registry, rect, ProjectileEntity::Type::AUTO_CANNON_PROJECTILE);
                }
                state = WeaponState::FIRING;
                GraphicsComponent::changeAsepriteTag(sprite, firing_tag, key);
                if (sound > 0) 
                    PlaySound(ResourceSystem::getSound("attack_01"));
                else
                    PlaySound(ResourceSystem::getSound("attack_02"));
            }
        }
    
        void animationCallback(entt::registry &registry, const entt::entity entity)
        {
            const unsigned int idle_tag = 0;
            const std::string_view key = "auto_cannon";

            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
            auto &state = registry.get<WeaponState>(entity);
            GraphicsComponent::changeAsepriteTag(sprite, idle_tag, key);
            state = WeaponState::IDLE;
        }

        void create(entt::registry &object_registry, const Rectangle rect, const entt::entity weapon_entity)
        {
            const int firing_tag_id = 1;
            const int firing_last_frame = 5;
            const std::string_view key = "auto_cannon";

            auto &input_container = object_registry.emplace<InputComponent::Container>(weapon_entity);

            object_registry.emplace<Rectangle>(weapon_entity, rect);
            object_registry.emplace<WeaponState>(weapon_entity, WeaponState::IDLE);
            object_registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);
            
            InputComponent::create(input_container, fireAutoCannon, KEY_SPACE, InputComponent::Type::DOWN);
            GraphicsComponent::addAnimationComponent(object_registry, weapon_entity, key, 0, rect, GraphicsComponent::RenderPriority::HIGH);
            GraphicsComponent::addCallback(object_registry, weapon_entity, firing_tag_id, firing_last_frame, animationCallback);
            
            Collision::addDynamicCollisionFromAseprite(object_registry, weapon_entity, key, false, Collision::Type::NONE, nullptr);
        }
    }
    namespace FighterWeapon {
        void animationCallback(entt::registry &registry, const entt::entity entity)
        {
            const unsigned int idle_tag = 1;
            const std::string_view key = "fighter_weapon";

            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
            auto &state = registry.get<WeaponState>(entity);
            GraphicsComponent::changeAsepriteTag(sprite, idle_tag, key);
            state = WeaponState::IDLE;
        }

        void fire(entt::registry &registry, const entt::entity entity)
        {
            const int weapon_reload_timer = 1;
            const unsigned int firing_tag = 0;
            const std::string_view key = "fighter_weapon";

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
                GraphicsComponent::changeAsepriteTag(sprite, firing_tag, key);
                TimerComponent::reset(timer_container, weapon_reload_timer);
            }
        }

        entt::entity create(entt::registry &object_registry, const Rectangle rect, const float fire_cooldown)
        {
            const entt::entity weapon_entity = object_registry.create();
            const int tag_id = 0;
            const int last_firing_frame = 3;
            const std::string_view key = "fighter_weapon";
            const int timer_id = 1;
            auto &timer_container = object_registry.emplace<TimerComponent::Container>(weapon_entity);

            object_registry.emplace<Rectangle>(weapon_entity, rect);
            object_registry.emplace<WeaponState>(weapon_entity, WeaponState::IDLE);
            object_registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);

            GraphicsComponent::addAnimationComponent(object_registry, weapon_entity, key, 1, rect, GraphicsComponent::RenderPriority::HIGH);
            GraphicsComponent::addCallback(object_registry, weapon_entity, tag_id, last_firing_frame, animationCallback);

            TimerComponent::createFinishedTimerInContainer(timer_container, fire_cooldown, timer_id);
            Collision::addDynamicCollisionFromAseprite(object_registry, weapon_entity, key, false, Collision::Type::NONE, nullptr);
            return weapon_entity;
        }
    }

    namespace ScoutWeapon {
        void animationCallback(entt::registry &registry, const entt::entity entity)
        {
            const int idle_tag_id = 0;
            const std::string_view key = "scout_weapon";
            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
            auto &state = registry.get<WeaponState>(entity);
            GraphicsComponent::changeAsepriteTag(sprite, idle_tag_id, key);
            state = WeaponState::IDLE;
        }

        void fire(entt::registry &registry, const entt::entity entity)
        {
            const int weapon_reload_timer = 1;
            const int firing_tag = 1;
            const std::string_view key = "scout_weapon";

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
                GraphicsComponent::changeAsepriteTag(sprite, firing_tag, key);
                TimerComponent::reset(timer_container, weapon_reload_timer);
            }
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
            auto &timer_container = registry.emplace<TimerComponent::Container>(weapon_entity);

            registry.emplace<Rectangle>(weapon_entity, rect);
            registry.emplace<WeaponState>(weapon_entity, WeaponState::IDLE);
            registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);

            GraphicsComponent::addAnimationComponent(registry, weapon_entity, key, initial_tag_idle, rect, GraphicsComponent::RenderPriority::HIGH);
            GraphicsComponent::addCallback(registry, weapon_entity, firing_tag_id, last_firing_frame, animationCallback);
            TimerComponent::createFinishedTimerInContainer(timer_container, fire_cooldown, timer_id);
            Collision::addDynamicCollisionFromAseprite(registry, weapon_entity, key, false, Collision::Type::NONE, nullptr);
            return weapon_entity;
        }
    }

    namespace BigSpaceGun {

        void initAnimationCallback(entt::registry &registry, const entt::entity entity)
        {
            const unsigned int idle_tag = 2;
            const std::string_view key = "big_space_gun";

            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
            auto &state = registry.get<WeaponState>(entity);

            GraphicsComponent::changeAsepriteTag(sprite, idle_tag, key);
            state = WeaponState::IDLE;
        }
        
        void chargeShot(entt::registry &registry, const entt::entity entity)
        {
            const unsigned int firing_tag = 0;
            const std::string_view key = "big_space_gun";
            auto &state = registry.get<WeaponState>(entity);
            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);

            if (state == WeaponState::IDLE) {
                state = WeaponState::FIRING;
                GraphicsComponent::changeAsepriteTag(sprite, firing_tag, key);
            }
        }

        void fire(entt::registry &registry, const entt::entity entity)
        {
            const int sound = GetRandomValue(0, 1);
            const int idle_tag = 2;
            const std::string_view key = "big_space_gun";
            
            auto &state = registry.get<WeaponState>(entity);
            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
            auto rect = registry.get<Collision::DynamicComponent>(entity).hitboxes[0].rect; // get hitboxes from container
            rect.height *= 6;

            ProjectileEntity::create(registry, rect, ProjectileEntity::Type::BIG_SPACE_GUN_PROJECTILE);
            if (sound > 0) 
                PlaySound(ResourceSystem::getSound("attack_01"));
            else
                PlaySound(ResourceSystem::getSound("attack_02"));
            state = WeaponState::IDLE;
            GraphicsComponent::changeAsepriteTag(sprite, idle_tag, key);
        }
        
        void create(entt::registry &registry, const Rectangle rect, const entt::entity weapon_entity)
        {
            const int init_tag = 1;
            const int last_init_frame = 10;
            const int fire_tag = 0;
            const int last_fire_frame = 6;

            const std::string_view key = "big_space_gun";
            auto &input_container = registry.emplace<InputComponent::Container>(weapon_entity);

            registry.emplace<Rectangle>(weapon_entity, rect);
            registry.emplace<WeaponState>(weapon_entity, WeaponState::INIT);
            registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);
            
            InputComponent::create(input_container, chargeShot, KEY_SPACE, InputComponent::Type::DOWN);
            GraphicsComponent::addAnimationComponent(registry, weapon_entity, key, init_tag, rect, GraphicsComponent::RenderPriority::HIGH);
            GraphicsComponent::addCallback(registry, weapon_entity, init_tag, last_init_frame, initAnimationCallback);
            GraphicsComponent::addCallback(registry, weapon_entity, fire_tag, last_fire_frame, fire);
            Collision::addDynamicCollisionFromAseprite(registry, weapon_entity, key, false, Collision::Type::NONE, nullptr);
        }
    }

    namespace Zapper {
        void initAnimationCallback(entt::registry &registry, const entt::entity entity)
        {
            const unsigned int idle_tag = 0;
            const std::string_view key = "zapper";
            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
            auto &state = registry.get<WeaponState>(entity);
            GraphicsComponent::changeAsepriteTag(sprite, idle_tag, key);
            state = WeaponState::IDLE;
        }
        
        void fireAnimationCallback(entt::registry &registry, const entt::entity entity)
        {
            const int idle_tag = 0;
            const std::string_view key = "zapper";
            auto &state = registry.get<WeaponState>(entity);
            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
            GraphicsComponent::changeAsepriteTag(sprite, idle_tag, key);
            state = WeaponState::IDLE;
        }

        void fire(entt::registry &registry, const entt::entity entity)
        {
            auto &state = registry.get<WeaponState>(entity);
            if (state == WeaponState::FIRING) {
                const int sound = GetRandomValue(0, 1);
                const Vector2 resolution = Graphics::getCurrentResolution();
                const auto &hitboxes = registry.get<Collision::DynamicComponent>(entity).hitboxes; // get hitboxes from container
                                                                                                   
                for (const auto hitbox : hitboxes) {
                    Rectangle rect = hitbox.rect;
                    rect.x -= rect.width;
                    rect.width = resolution.x / 20.f;
                    rect.height = resolution.y / 20.f;
                    ProjectileEntity::create(registry, rect, ProjectileEntity::Type::ZAPPER_PROJECTILE);
                }
                if (sound > 0) 
                    PlaySound(ResourceSystem::getSound("attack_01"));
                else
                    PlaySound(ResourceSystem::getSound("attack_02"));
                state = WeaponState::DECHARGE;
            }
        }

        void chargeShot(entt::registry &registry, const entt::entity entity)
        {
            auto &state = registry.get<WeaponState>(entity);
            auto &sprite = registry.get<GraphicsComponent::Animation>(entity);
                     
            if (state == WeaponState::IDLE) {
                const int firing_tag = 1;
                const std::string_view key = "zapper";
                GraphicsComponent::changeAsepriteTag(sprite, firing_tag, key);
                state = WeaponState::FIRING;
            }
        }

        void create(entt::registry &registry, const Rectangle rect, const entt::entity weapon_entity)
        {
            const int firing_tag = 1;
            const int init_tag = 2;
            const int firing_frame = 6;
            const int last_init_frame = 12;
            const int last_firing_frame = 8;

            const std::string_view key = "zapper";
            auto &input_container = registry.emplace<InputComponent::Container>(weapon_entity);

            registry.emplace<Rectangle>(weapon_entity, rect);
            registry.emplace<WeaponState>(weapon_entity, WeaponState::INIT);
            registry.emplace<ObjectType>(weapon_entity, ObjectType::SHIP_COMPONENT);
            
            InputComponent::create(input_container, chargeShot, KEY_SPACE, InputComponent::Type::DOWN);
            GraphicsComponent::addAnimationComponent(registry, weapon_entity, key, init_tag, rect, GraphicsComponent::RenderPriority::HIGH);
            GraphicsComponent::addCallback(registry, weapon_entity, init_tag, last_init_frame, initAnimationCallback);
            GraphicsComponent::addCallback(registry, weapon_entity, firing_tag, firing_frame, fire);
            GraphicsComponent::addCallback(registry, weapon_entity, firing_tag, last_firing_frame, fireAnimationCallback);
            Collision::addDynamicCollisionFromAseprite(registry, weapon_entity, key, false, Collision::Type::NONE, nullptr);
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
    case SCOUT_WEAPON:
        return ScoutWeapon::create(registry, rect);
    }
    return entt::null;
}

void WeaponEntity::fire(entt::registry &registry, const entt::entity entity, const EnemyType type)
{
    switch(type) {
    using enum WeaponEntity::EnemyType;
    case FIGHTER_WEAPON:
        FighterWeapon::fire(registry, entity);
        break;
    case SCOUT_WEAPON:
        ScoutWeapon::fire(registry, entity);
        break;
    }
}
