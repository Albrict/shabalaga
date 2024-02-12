#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"
#include "engine_entity.hpp"
#include "weapon_entity.hpp"

namespace PlayerEntity {
    entt::entity create(entt::registry &object_registry, const Rectangle rect, const WeaponEntity::PlayerType weapon_type, 
                        const EngineEntity::PlayerType type);
    void destroy(entt::registry &registry, const entt::entity player);
};

