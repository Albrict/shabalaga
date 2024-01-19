#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace ScoutWeapon {
    entt::entity create(entt::registry &registry, const Rectangle rect); 
    void fire(entt::registry &registry, const entt::entity entity);
}
