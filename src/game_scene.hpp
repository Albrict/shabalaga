#pragma once
#include "scene.hpp"
#include "../include/entt.hpp"
class GameScene : public Scene {
public:
    GameScene();
    ~GameScene() = default;
    
    void proccessEvents() override;
    void update() override;
    void draw() const override;
private:
    entt::registry registry;
};
