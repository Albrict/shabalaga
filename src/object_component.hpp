#pragma once
#include "../include/raylib.h"
#include "../include/entt.hpp"

enum class ObjectType {
    PLAYER_SHIP,
    ENEMY_SHIP,
    SHIP_COMPONENT,
    PROJECTILE,
    EXPLOSION,
    ENEMY_PROJECTILE,
    WIDGET,
    GAME_MASTER,
    PICK_UP,
    HUD
};

enum class WeaponState {
    INIT,
    IDLE,
    FIRING,
};

struct VelocityComponent {
    Vector2 velocity;
};

struct HealthComponent {
    int health;
};

struct DamageComponent {
    int damage;
};

namespace ObjectComponent {
    struct Score {
        const int score;
    };

    struct PlayerPosition {
        Rectangle rect;
    };
}
