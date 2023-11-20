#pragma once
#include "../include/entt.hpp"

class Scene {
protected:
    struct MessageEmiter : entt::emitter<MessageEmiter> {};
    MessageEmiter emmiter;
public:
    using Emmiter = entt::emitter<MessageEmiter>;
    enum class Messages {
        PLAY,
        SETTINGS,
        EXIT,
        MAIN_MENU,
        NONE
    };
    Scene() = default;
    virtual ~Scene()
    { emmiter.clear(); }

    virtual void proccessEvents() = 0;
    virtual void update() = 0;
    virtual void draw() const = 0;
    
    MessageEmiter &getEmmiter() noexcept
    { return emmiter; }
};
