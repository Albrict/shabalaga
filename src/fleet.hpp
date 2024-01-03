#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace Fleet {
    enum class Type {
        FIGHTER_FLEET_LONG_LINE,
        FIGHTER_FLEET_SHORT_LINE
    };

    void createFleet(entt::registry &registry, const Vector2 position, const size_t ship_amount, const Type type);
}   
