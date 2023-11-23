#pragma once
#include "../include/raylib.h"

enum class ShipType {
    PLAYER
};

struct ShipComponent {
    Rectangle hitbox;
    Vector2 position;
    Vector2 velocity;
    int health;
    ShipComponent(const Rectangle hitbox, const Vector2 position, const Vector2 velocity, const int health)
        : hitbox(hitbox),
        position(position),
        velocity(velocity),
        health(health) {}
};
