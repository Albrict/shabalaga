#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace Collision {
    typedef void(*callback)(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity);
    enum class Type {
        BOUNDS, // checks if object is hit bound
        OUT_OF_BOUNDS, // checks if object is out of bounds
        CUSTOM_BOUNDS,
        NONE
    };
    
    struct Hitbox {
        Rectangle rect {};
        float x_padding = 0.f;
        float y_padding = 0.f;
    };

    struct DynamicComponent {
        bool visible = false; // if visibility false, then don't check with other objects
        float custom_bound_x = 0.f;
        float custom_bound_y = 0.f;
        std::vector<Hitbox> hitboxes {};
        Type type = Type::NONE;
        callback onCollide = nullptr;
    };

    struct StaticComponent {
        bool visible = false; // if visibility false, then don't check with other objects
        std::vector<Hitbox> hitboxes {};
        callback onCollide = nullptr;
    };
    
    void addDynamicCollisionFromAseprite(entt::registry &registry, const entt::entity entity, 
            const std::string_view &key, const bool visibile, const Type type, callback onCollide);
    void addDynamicCollisionFromAseprite(entt::registry &registry, const entt::entity entity, 
            const std::string_view &key, const bool visibile, 
            const float custom_bound_x, const float custom_bound_y, callback onCollide);
    void addStaticCollisionFromAseprite(entt::registry &registry, const entt::entity, 
            const std::string_view &key, bool visibile, callback onCollide);
    void addStaticCollision(entt::registry &registry, const entt::entity entity, 
            const std::string_view &key, std::vector<Hitbox> hitboxes, bool visibile, callback onCollide);
}
