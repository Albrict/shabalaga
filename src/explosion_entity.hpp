#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace ExplosionEntity {
    enum Type : size_t {
        PLAYER_EXPLOSION = 0,
        FIGHTER_EXPLOSION,
        BOMBER_EXPLOSION,
        SCOUT_EXPLOSION,
        SMALL_EXPLOSION,
        BSG_EXPLOSION 
    };
    void create(entt::registry &registry, const Rectangle rect, const Type type);  
}
