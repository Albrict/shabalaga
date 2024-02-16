#pragma once
#include "../include/entt.hpp"
#include "weapon_entity.hpp"
#include "engine_entity.hpp"

namespace GameMasterComponent {
    struct GameInfo {
        enum class Difficulty {
            EASY,
            PRE_MEDIUM,
            MEDIUM,
            PRE_HARD,
            HARD,
            BOSS
        };
        Difficulty current_difficulty = Difficulty::EASY;
        entt::entity player_entity = entt::null;
        int score = 0;
        int scout_amount = 0;
        WeaponEntity::PlayerType player_weapon = WeaponEntity::PlayerType::AUTO_CANNON;
        EngineEntity::PlayerType player_engine = EngineEntity::PlayerType::BASIC;
    };
}
