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
        int score = 0;
        int scout_amount = 0;
        int engine_type = 0;
        int weapon_type = 0;
    };
}
