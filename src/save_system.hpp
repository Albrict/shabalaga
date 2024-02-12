#pragma once
#include "engine_entity.hpp"
#include "weapon_entity.hpp"
#include <deque>
#include <optional>

namespace SaveSystem {
    struct Save {
        int score;
        WeaponEntity::PlayerType equiped_weapon;
        EngineEntity::PlayerType equiped_engine;
            
        std::deque<bool> weapons {};
        std::deque<bool> engines {};
    };
    std::optional<Save> load(const char *path); 
    void save(Save &data, const char *path);
}
