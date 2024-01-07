#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace PlayerEntity {
    entt::entity create(entt::registry &object_registry, const Vector2 position, const float width, const float height);
};

