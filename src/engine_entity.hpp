#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace EngineEntity {
    
    enum PlayerType : int {
        BASIC = 0,
        BIG_PULSE_ENGINE,
        BURST_ENGINE,
        SUPERCHARGED_ENGINE
    };

    enum EnemyType : int {
        FIGHTER,
        BOMBER,
        SCOUT
    };
    entt::entity create(entt::registry &object_registry, const PlayerType type, const Rectangle rect);
    entt::entity create(entt::registry &object_registry, const EnemyType type, const Rectangle rect);
};

