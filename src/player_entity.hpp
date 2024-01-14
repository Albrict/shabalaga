#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace PlayerEntity {
    entt::entity create(entt::registry &object_registry, const Rectangle rect);
    void destroy(entt::registry &registry, const entt::entity player);
};

