#include "fuel_pickup_entity.hpp"
#include "clean_up_component.hpp"
#include "collision_component.hpp"
#include "graphics_component.hpp"
#include "graphics.hpp"
#include "message_system.hpp"
#include "object_component.hpp"

namespace FuelPickUpEntity {
    void collisionCallback(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity)
    {
        const auto type = registry.get<ObjectType>(b_entity);
        if (type == ObjectType::PLAYER_SHIP) {
            MessageSystem::Message message = {
                .msg = MessageSystem::HudMessage::INCREASE_FUEL,
                .type = MessageSystem::Type::HUD_MESSAGE
            };
            MessageSystem::sendMessageToEntity(registry, message);
            registry.emplace<CleanUpComponent::Component>(a_entity);
            const int rand_sound = GetRandomValue(0, 1);
            if (rand_sound)
                PlaySound(ResourceSystem::getSound("fuel_pickup_01"));
            else
                PlaySound(ResourceSystem::getSound("fuel_pickup_02"));
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

    object_registry.emplace<Rectangle>(fuel, rect);

    object_registry.emplace<VelocityComponent>(fuel, velocity);
    object_registry.emplace<ObjectType>(fuel, ObjectType::PICK_UP);

    GraphicsComponent::addAnimationComponent(object_registry, fuel, key, 0, rect, GraphicsComponent::RenderPriority::LOW);
    Collision::addDynamicCollisionFromAseprite(object_registry, fuel, key, true, Collision::Type::OUT_OF_BOUNDS, collisionCallback);
}
