#pragma once
#include "../include/entt.hpp"

namespace ButtonSystem {
    void proccessEvents(const entt::registry &registry);
    void update(entt::registry &registry);
    void draw(const entt::registry &registry);
};
