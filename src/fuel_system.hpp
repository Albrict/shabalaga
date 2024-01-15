#pragma once
#include "../include/entt.hpp"

namespace FuelSystem {
    void reset();
    void increaseFuelLevel();
    void proccessEvents(entt::registry &registry, const entt::entity entity);
}
