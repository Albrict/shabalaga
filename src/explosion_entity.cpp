#include "explosion_entity.hpp"
#include "clean_up_component.hpp"
#include "collision_component.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "message_system.hpp"
#include "object_component.hpp"

namespace ExplosionEntity {
    std::array<const std::string_view, 7> sprite_keys = {
        "player_explosion",
        "fighter_explosion",
        "bomber_explosion",
        "scout_explosion",
        "small_explosion",
        "bsg_explosion"
    };
    
    void explosionCallback(entt::registry &registry, const entt::entity entity)
    {
        registry.emplace<CleanUpComponent::Component>(entity);
    }

    namespace PlayerExplosion {
        void animationCallback(entt::registry &registry, const entt::entity entity)
        {
            registry.emplace<CleanUpComponent::Component>(entity);
            MessageSystem::Message msg = {.msg = MessageSystem::PlaySceneMessage::PLAYER_DIED, 
            .type = MessageSystem::Type::PLAY_SCENE_MESSAGE};
            MessageSystem::sendMessage(msg);
        }

        void create(entt::registry &registry, const Rectangle rect)
        {
            const auto explosion = registry.create();
            const int explosion_tag_id = 0;
            const int last_frame = 8;
            const std::string_view key = sprite_keys[PLAYER_EXPLOSION];

            registry.emplace<Rectangle>(explosion, rect);

            GraphicsComponent::addAnimationComponent(registry, explosion, key, explosion_tag_id, rect, GraphicsComponent::RenderPriority::HIGH);
            GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, animationCallback);
        }
    }
    
    namespace BomberExplosion {
        void animationCallbackRemoveComponents(entt::registry &registry, const entt::entity entity)
        {
            registry.remove<Collision::DynamicComponent>(entity);
        }
        
        void animationCallback(entt::registry &registry, const entt::entity entity)
        {
            registry.emplace<CleanUpComponent::Component>(entity);
        }

        void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity) 
        {
            const auto a_type = registry.get<ObjectType>(a_entity);
            const auto b_type = registry.get<ObjectType>(b_entity);
            if (a_type == ObjectType::EXPLOSION) {
                if (b_type == ObjectType::PLAYER_SHIP ||b_type == ObjectType::ENEMY_SHIP) {
                    const int damage = registry.get<DamageComponent>(a_entity).damage;
                    auto &health = registry.get<HealthComponent>(b_entity);
                    health.health -= damage; 
                }
            }
        }

        void create(entt::registry &registry, const Rectangle rect)
        {
            const auto explosion = registry.create();
            const int explosion_tag_id = 0;
            const int last_frame = 8;
            const int last_explosion_frame = 5;
            const std::string_view key = "bomber_explosion";

            registry.emplace<Rectangle>(explosion, rect);
            registry.emplace<DamageComponent>(explosion, 100);
            registry.emplace<ObjectType>(explosion, ObjectType::EXPLOSION);

            GraphicsComponent::addAnimationComponent(registry, explosion, key, 0, rect, GraphicsComponent::RenderPriority::LOW);
            GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, animationCallback);
            GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_explosion_frame, animationCallbackRemoveComponents);
            Collision::addDynamicCollisionFromAseprite(registry, explosion, key, true, Collision::Type::OUT_OF_BOUNDS, collisionCallback);
        }
    }
    
    namespace SmallExplosion {
        entt::entity create(entt::registry &registry, const Rectangle rect)
        {
            const entt::entity explosion = registry.create();
            const int explosion_tag_id = 0;
            const int last_frame = 2;
            const std::string_view key = sprite_keys[SMALL_EXPLOSION]; 

            registry.emplace<Rectangle>(explosion, rect);
            GraphicsComponent::addAnimationComponent(registry, explosion, key, 0, rect, GraphicsComponent::RenderPriority::LOW);
            GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, explosionCallback);
            return explosion;
        }
    }
        
    namespace BSG_Explosion {
        void animationCallback(entt::registry &registry, const entt::entity entity)
        {
            registry.emplace<CleanUpComponent::Component>(entity);
        }

        void createExplosionAnimation(entt::registry &registry, const Rectangle rect)
        {
            const Vector2 resolution = Graphics::getCurrentResolution();
            const auto explosion = registry.create();
            const int explosion_tag_id = 0;
            const int last_frame = 6;
            const std::string_view key = sprite_keys[BSG_EXPLOSION];
            registry.emplace<Rectangle>(explosion, rect);

            GraphicsComponent::addAnimationComponent(registry, explosion, key, 0, rect, GraphicsComponent::RenderPriority::LOW);
            GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, animationCallback);
        }

        void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity) 
        {
            const auto a_type = registry.get<ObjectType>(a_entity);
            const auto b_type = registry.get<ObjectType>(b_entity);
            if (a_type == ObjectType::EXPLOSION && b_type == ObjectType::ENEMY_SHIP) {
                auto rect = registry.get<Rectangle>(b_entity);
                rect.width /= 2.f;
                rect.height /= 2.f;
                rect.x += rect.width;
                rect.y += rect.height;
                createExplosionAnimation(registry, rect);
            }
        }

        void create(entt::registry &registry, const Rectangle rect)
        {
            const Vector2 resolution = Graphics::getCurrentResolution();
            const auto explosion = registry.create();
            const int explosion_tag_id = 0;
            const int last_frame = 6;
            const std::string_view key = sprite_keys[BSG_EXPLOSION];
            std::vector<Collision::Hitbox> hitboxes;
            const Collision::Hitbox hitbox = {
                .rect = {
                    .x = rect.x - rect.width * 2.f,
                    .y = rect.y,
                    .width = rect.width * 4.f,
                    .height = rect.height 
                }
            };
            hitboxes.push_back(hitbox);

            registry.emplace<DamageComponent>(explosion, 200);
            registry.emplace<Rectangle>(explosion, rect);
            registry.emplace<ObjectType>(explosion, ObjectType::EXPLOSION);

            GraphicsComponent::addAnimationComponent(registry, explosion, key, 0, rect, GraphicsComponent::RenderPriority::LOW);
            GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, animationCallback);
            Collision::addStaticCollision(registry, explosion, key, hitboxes, true, collisionCallback);
        }
    }
}

void ExplosionEntity::create(entt::registry &registry, const Rectangle rect, const Type type)
{
    switch(type) {
    case Type::SMALL_EXPLOSION:
        SmallExplosion::create(registry, rect);
        return;
    case Type::BSG_EXPLOSION:
        BSG_Explosion::create(registry, rect);
        return;
    case Type::BOMBER_EXPLOSION:
        BomberExplosion::create(registry, rect);
        return;
    case Type::PLAYER_EXPLOSION:
        PlayerExplosion::create(registry, rect);
        return;
    default:
        const auto explosion = registry.create();
        const int explosion_tag_id = 0;
        const int last_frame = 8;
        const std::string_view key = sprite_keys[type]; 

        registry.emplace<Rectangle>(explosion, rect);
        GraphicsComponent::addAnimationComponent(registry, explosion, key, explosion_tag_id, rect, GraphicsComponent::RenderPriority::HIGH);
        GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, explosionCallback);
    }
}
