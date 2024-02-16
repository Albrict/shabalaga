#pragma once
#include "../include/entt.hpp"

namespace CollisionSystem {
    void update(entt::registry &registry);
    void updateHitboxes(entt::registry &registry);
};
