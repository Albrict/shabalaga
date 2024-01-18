#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace FighterEntity {
    entt::entity create(entt::registry &object_registry, const Rectangle rect, const float fire_cooldown);
}
