#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace ProjectileEntity {
    enum class Type {
        AUTO_CANNON_PROJECTILE,
        BIG_SPACE_GUN_PROJECTILE,
        BULLET_PROJECTILE,
    };

    entt::entity create(entt::registry &registry, const Rectangle rect, const Type type);
}
