#pragma once
#include "../include/entt.hpp"

namespace PlayerSystem {
    void proccessEvents(entt::registry &registry);
    void update(entt::registry &registry);
    void draw(const entt::registry &registry);
}
