#pragma once
#include "sprite.hpp"
#include <memory>

enum class WeaponType {
    AUTO_CANNON,
    BIG_SPACE_GUN,
    ZAPPER
};

enum class WeaponState {
    INIT,
    IDLE,
    FIRING,
};

struct WeaponComponent {
    Sprite sprite;
    WeaponType type;
    WeaponState state;
};
