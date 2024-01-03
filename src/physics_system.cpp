#include "physics_system.hpp"
#include "collision_system.hpp"
#include "object_component.hpp"
#include "../include/raymath.h"


namespace PhysicsSystem {
    void applyVelocity(entt::registry &registry)
    {
        auto view = registry.view<VelocityComponent, Rectangle>();
        for (auto [entity, velocity, rect] : view.each()) {
            Vector2 position = { rect.x, rect.y };
            Vector2 delta_velocity = velocity.velocity;
            delta_velocity.x *= GetFrameTime();
            delta_velocity.y *= GetFrameTime();
            position = Vector2Add(position, delta_velocity);
            rect.x = position.x;
            rect.y = position.y;
        }
    }
    
}

void PhysicsSystem::update(entt::registry &registry)
{
    applyVelocity(registry);
    CollisionSystem::update(registry);
}
