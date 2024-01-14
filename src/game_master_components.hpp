#pragma once
#include "../include/entt.hpp"

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
    };
}
