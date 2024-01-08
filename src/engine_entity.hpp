#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace EngineEntity {
    enum class Type {
        BASIC,
        FIGHTER
    };

    entt::entity create(entt::registry &object_registry, const Type type, const Rectangle rect);
};

