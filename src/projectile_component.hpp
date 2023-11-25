#pragma once
#include "sprite.hpp"
#include <memory>

enum class ProjectileType {
    AUTO_CANNON_PROJECTILE
};

struct ProjectileComponent {
    Sprite sprite;
    ProjectileType type;
    int damage;
    Vector2 position;
};
