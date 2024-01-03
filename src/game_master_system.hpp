#pragma once
#include "../include/entt.hpp"

namespace GameMasterSystem {
    void proccessEvents(entt::registry &registry, const entt::entity game_master);
    void update(entt::registry &registry, const entt::entity game_master);
}
