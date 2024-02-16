#pragma once
#include "../include/entt.hpp"
#include "message_system.hpp"

class Scene {
public:
    Scene() = default;
    virtual ~Scene() = default;
    
    virtual void proccessEvents() = 0;
    virtual void update() = 0;
    virtual void draw() const = 0;

    void sendMessage(MessageSystem::Message &msg)
    { received_msg = msg; is_received = true; }
protected:
    bool is_received = false;
    MessageSystem::Message received_msg {};
    entt::registry object_registry;
};
