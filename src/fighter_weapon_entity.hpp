#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace FighterWeaponEntity {
    void fire(entt::registry &registry, const entt::entity entity);
    entt::entity create(entt::registry &registry, const Rectangle rect);
}
