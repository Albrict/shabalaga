#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace BomberEntity {
    void create(entt::registry &registry, const Rectangle rect, const Rectangle last_player_position);
    void destroy(entt::registry &registry, const entt::entity entity);
}
