#include "explosion_entity.hpp"
#include "clean_up_component.hpp"
#include "collison_component.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
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
            registry.remove<CollisionComponent::Component>(entity);
            registry.remove<HitboxComponent::Container>(entity);
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
            auto &collider = registry.emplace<CollisionComponent::Component>(explosion, 
                                 CollisionComponent::create(true, true, CollisionComponent::Type::NONE, collisionCallback));
            auto &container = registry.emplace<HitboxComponent::Container>(explosion, explosion);
           
            registry.emplace<DamageComponent>(explosion, 100);
            registry.emplace<Rectangle>(explosion, rect);

            registry.emplace<ObjectType>(explosion, ObjectType::EXPLOSION);

            GraphicsComponent::addAnimationComponent(registry, explosion, key, 0, rect, GraphicsComponent::RenderPriority::LOW);
            GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, animationCallback);
            GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_explosion_frame, animationCallbackRemoveComponents);
            HitboxComponent::loadHitboxesInContainer(container, key, rect);
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
            HitboxComponent::Hitbox hitbox;
            hitbox.rect = rect;
            hitbox.rect.width = resolution.x / 3.f;
            hitbox.rect.height *= 1.5f;
            hitbox.rect.x = -resolution.x;
            hitbox.rect.y -= hitbox.rect.height;

            hitbox.x_padding = -resolution.x / 8.f;
            hitbox.y_padding = 0.f;

            auto &collider = registry.emplace<CollisionComponent::Component>(explosion, 
                                 CollisionComponent::create(true, true, CollisionComponent::Type::NONE, collisionCallback));
            auto &container = registry.emplace<HitboxComponent::Container>(explosion, explosion);
            container.hitboxes.push_back(hitbox); 
            registry.emplace<DamageComponent>(explosion, 200);
            registry.emplace<Rectangle>(explosion, rect);
            
            registry.emplace<ObjectType>(explosion, ObjectType::EXPLOSION);
            GraphicsComponent::addAnimationComponent(registry, explosion, key, 0, rect, GraphicsComponent::RenderPriority::LOW);
            GraphicsComponent::addCallback(registry, explosion, explosion_tag_id, last_frame, animationCallback);
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
