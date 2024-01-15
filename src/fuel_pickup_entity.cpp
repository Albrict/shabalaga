#include "fuel_pickup_entity.hpp"
#include "clean_up_component.hpp"
#include "collison_component.hpp"
#include "fuel_system.hpp"
#include "graphics_component.hpp"
#include "hitbox_component.hpp"
#include "graphics.hpp"
#include "object_component.hpp"

namespace FuelPickUpEntity {
    void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity)
    {
        const auto type = registry.get<ObjectType>(b_entity);
        if (type == ObjectType::PLAYER_SHIP) {
            FuelSystem::increaseFuelLevel();   
            registry.emplace<CleanUpComponent::Component>(a_entity);
        } else {
            return;
        }
    }
}

void FuelPickUpEntity::create(entt::registry &object_registry, const Rectangle rect)
{
    const entt::entity fuel = object_registry.create();     
    const std::string_view key = "fuel_pickup";
    const Vector2 velocity = {0.f, Graphics::getCurrentResolution().y / 5.f};

    auto &collider = object_registry.emplace<CollisionComponent::Component>(fuel);
    auto &container = object_registry.emplace<HitboxComponent::Container>(fuel, fuel);

    object_registry.emplace<Rectangle>(fuel, rect);
    object_registry.emplace<VelocityComponent>(fuel, velocity);
    object_registry.emplace<ObjectType>(fuel, ObjectType::PICK_UP);

    GraphicsComponent::addAnimationComponent(object_registry, fuel, key, 0, rect, GraphicsComponent::RenderPriority::LOW);
    collider = CollisionComponent::create(true, CollisionComponent::Type::OUT_OF_BOUNDS, collisionCallback, nullptr);
    HitboxComponent::loadHitboxesInContainer(container, key, rect);
}
