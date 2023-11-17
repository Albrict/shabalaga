#pragma once
#include "../include/entt.hpp"

namespace BackgroundSystem {
    void clear(entt::registry &registry);
    void update(entt::registry &registry);
    void draw(const entt::registry &registry);
};
