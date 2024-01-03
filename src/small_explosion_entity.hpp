#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace SmallExplosionEntity {
    entt::entity create(entt::registry &registry, const Rectangle rect);
}
