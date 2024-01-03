#pragma once
#include "../include/entt.hpp"

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;
    
    virtual void proccessEvents() = 0;
    virtual void update() = 0;
    virtual void draw() const = 0;
protected:
    entt::registry object_registry;
};
