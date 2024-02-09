#include "projectile_entity.hpp"
#include "clean_up_component.hpp"
#include "collision_component.hpp"
#include "explosion_entity.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "object_component.hpp"

namespace ProjectileEntity {
    namespace AutoCannonProjectile {
        void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity)
        {
            const auto type = registry.get<ObjectType>(b_entity);
            if (type == ObjectType::ENEMY_SHIP) {
                const Rectangle rect = registry.get<Rectangle>(a_entity);
                const int sound = GetRandomValue(0, 1);
                if (sound > 0)
                    PlaySound(ResourceSystem::getSound("hitsound_01"));
                else
                    PlaySound(ResourceSystem::getSound("hitsound_02"));
                registry.emplace_or_replace<CleanUpComponent::Component>(a_entity);
                ExplosionEntity::create(registry, rect, ExplosionEntity::Type::SMALL_EXPLOSION);
            } else {
                return;
            }
        }

        entt::entity create(entt::registry &object_registry, const Rectangle rect)
        {
            const entt::entity projectile_entity = object_registry.create();
            const Vector2 resolution = Graphics::getCurrentResolution();
            const Vector2 velocity = {0.f, resolution.y / -1.5f};
            const std::string_view key = "auto_cannon_projectile";

            object_registry.emplace<Rectangle>(projectile_entity, rect);
            object_registry.emplace<DamageComponent>(projectile_entity, 100);
            object_registry.emplace<VelocityComponent>(projectile_entity, velocity);
            object_registry.emplace<ObjectType>(projectile_entity, ObjectType::PROJECTILE);
            object_registry.emplace<Type>(projectile_entity, Type::AUTO_CANNON_PROJECTILE);

            GraphicsComponent::addAnimationComponent(object_registry, projectile_entity, key, 0, rect, GraphicsComponent::RenderPriority::HIGH);
            Collision::addDynamicCollisionFromAseprite(object_registry, projectile_entity, key, true, Collision::Type::OUT_OF_BOUNDS, collisionCallback);
            return projectile_entity;
        }
    }

    namespace BigSpaceGunProjectile {
        void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity)
        {
            const auto type = registry.get<ObjectType>(b_entity);
            if (type == ObjectType::ENEMY_SHIP) {
                Rectangle rect = registry.get<Rectangle>(a_entity);
                const int sound = GetRandomValue(0, 1);
                rect.width *= 2.f;
                rect.height *= 2.f;
                if (sound > 0)
                    PlaySound(ResourceSystem::getSound("hitsound_01"));
                else
                    PlaySound(ResourceSystem::getSound("hitsound_02"));
                registry.emplace_or_replace<CleanUpComponent::Component>(a_entity);
                ExplosionEntity::create(registry, rect, ExplosionEntity::Type::BSG_EXPLOSION);
            } else {
                return;
            }
        }

        entt::entity create(entt::registry &object_registry, const Rectangle rect)
        {
            const entt::entity projectile_entity = object_registry.create();
            const Vector2 resolution = Graphics::getCurrentResolution();
            const Vector2 velocity = {0.f, resolution.y / -1.5f};
            const std::string_view key = "big_space_gun_projectile";
            
            object_registry.emplace<Rectangle>(projectile_entity, rect);
            object_registry.emplace<DamageComponent>(projectile_entity, 100);
            object_registry.emplace<VelocityComponent>(projectile_entity, velocity);
            object_registry.emplace<ObjectType>(projectile_entity, ObjectType::PROJECTILE);
            object_registry.emplace<Type>(projectile_entity, Type::BIG_SPACE_GUN_PROJECTILE);

            GraphicsComponent::addAnimationComponent(object_registry, projectile_entity, key, 0, rect, GraphicsComponent::RenderPriority::HIGH);
            Collision::addDynamicCollisionFromAseprite(object_registry, projectile_entity, key, true, Collision::Type::OUT_OF_BOUNDS, collisionCallback);
            return projectile_entity;
        }
    }

    namespace BulletProjectile {
        void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity) 
        {
            const auto type = registry.get<ObjectType>(b_entity);
            if (type == ObjectType::PLAYER_SHIP) {
                // Create explosion
                const int sound = GetRandomValue(0, 1);
                Rectangle rect = registry.get<Rectangle>(a_entity);
                registry.emplace_or_replace<CleanUpComponent::Component>(a_entity);
                rect.width *= 6.f;
                rect.height *= 6.f;
                ExplosionEntity::create(registry, rect, ExplosionEntity::Type::SMALL_EXPLOSION);
                if (sound > 0)
                    PlaySound(ResourceSystem::getSound("player_hitsound_01"));
                else 
                    PlaySound(ResourceSystem::getSound("player_hitsound_02"));
            } else {
                return;
            }
        }    

        entt::entity create(entt::registry &object_registry, const Rectangle rect)
        {
            const entt::entity projectile_entity = object_registry.create();
            const Vector2 resolution = Graphics::getCurrentResolution();
            const Vector2 velocity = {0.f, resolution.y / 2.f};
            const std::string_view key = "bullet_projectile";
            const float custom_bound_x = rect.width * 8.f;
            const float custom_bound_y = rect.height * 8.f; 
            
            object_registry.emplace<Rectangle>(projectile_entity, rect);
            object_registry.emplace<DamageComponent>(projectile_entity, 33); 
            object_registry.emplace<VelocityComponent>(projectile_entity, velocity);
            object_registry.emplace<ObjectType>(projectile_entity, ObjectType::ENEMY_PROJECTILE);

            GraphicsComponent::addAnimationComponent(object_registry, projectile_entity, key, 0, rect, GraphicsComponent::RenderPriority::HIGH);
            Collision::addDynamicCollisionFromAseprite(object_registry, projectile_entity, key, true, custom_bound_x, custom_bound_y, collisionCallback);
            return projectile_entity;
        }
    }
}

entt::entity ProjectileEntity::create(entt::registry &registry, const Rectangle rect, const Type type)
{
    switch(type) {
    case Type::AUTO_CANNON_PROJECTILE:
        return AutoCannonProjectile::create(registry, rect);
        break;
    case Type::BIG_SPACE_GUN_PROJECTILE:
        return BigSpaceGunProjectile::create(registry, rect);
        break;
    case Type::BULLET_PROJECTILE:
        return BulletProjectile::create(registry, rect);
        break;
    }
    return entt::null;
}
