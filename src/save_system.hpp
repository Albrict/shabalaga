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

    struct Settings {
        float brightness = 0.f;
        float sound = 0.f;
        bool is_fullscreen = false;
    };

    std::optional<Save> load(const char *path); 
    std::optional<Settings> loadSettings(const char *path); 
    void save(Save &data, const char *path);
    void save(Settings &data, const char *path);
}
