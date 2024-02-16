#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace WeaponEntity {
    enum PlayerType : int {
        AUTO_CANNON = 0,
        ZAPPER,
        BIG_SPACE_GUN,
    };
    
    enum EnemyType {
        FIGHTER_WEAPON,
        SCOUT_WEAPON
    };

    entt::entity create(entt::registry &registry, const Rectangle rect, const PlayerType type);
    entt::entity create(entt::registry &registry, const Rectangle rect, const EnemyType type, const float cooldown = 0.f);
    
    void fire(entt::registry &registry, const entt::entity entity, const EnemyType type);
}
