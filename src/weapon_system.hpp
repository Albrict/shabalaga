#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace WeaponSystem {
    void proccessEvents(entt::registry &registry);
    void update(entt::registry &registry);
    void draw(const entt::registry &registry, const Rectangle dest);
};
