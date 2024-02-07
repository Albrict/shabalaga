#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace CollisionComponent {
    typedef void(*callback)(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity);
    enum class Type {
        BOUNDS, // checks if object is hit bound
        OUT_OF_BOUNDS, // checks if object is out of bounds
        CUSTOM_BOUNDS,
        NONE
    };
    struct Component {
        bool visible = false;
        bool high_preccision = false;
        Type type = Type::NONE;
        callback onCollide = nullptr;
        float custom_bound_x = 0.f;
        float custom_bound_y = 0.f;
    };
    
    inline Component create(const bool visibility = true, const bool high_preccision = false, const Type type = Type::NONE, callback onCollide = nullptr)
    {
        return (Component){
            .visible = visibility,
            .type = type,
            .onCollide = onCollide,
        };
    }

    inline Component create(const bool visibility = true, const bool high_preccision = false, const float custom_bound_x = 0.f, const float custom_bound_y = 0.f,
            callback onCollide = nullptr)
    {
        return (Component){
            .visible = visibility,
            .type = Type::CUSTOM_BOUNDS,
            .onCollide = onCollide,
            .custom_bound_x = custom_bound_x,
            .custom_bound_y = custom_bound_y
        };
    }
}

namespace Collision {
    typedef void(*callback)(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity);
    enum class Type {
        BOUNDS, // checks if object is hit bound
        OUT_OF_BOUNDS, // checks if object is out of bounds
        CUSTOM_BOUNDS,
        NONE
    };

    struct Component {
        bool visible = false; // if visibility false, then don't check with other objects
        Rectangle box {};
        Type type = Type::NONE;
        callback onCollide = nullptr;
        float custom_bound_x = 0.f;
        float custom_bound_y = 0.f;
    };
}
