#pragma once
#include "../include/entt.hpp"

namespace CollisionComponent {
    typedef void(*callback)(entt::registry &registry, const entt::entity a_entity, const entt::entity b_entity);
    enum class Type {
        BOUNDS, // checks if object is hit bound
        OUT_OF_BOUNDS, // checks if object is out of bounds
        NONE
    };
    struct Component {
        bool visible = false;
        Type type = Type::NONE;
        callback onCollide = nullptr;
        callback onExit = nullptr;
    };
    
    inline Component create(const bool visibility, const Type type, callback onCollide = nullptr, callback onExit = nullptr)
    {
        return (Component){
            .visible = visibility,
            .type = type,
            .onCollide = onCollide,
            .onExit = onExit
        };
    }
}
