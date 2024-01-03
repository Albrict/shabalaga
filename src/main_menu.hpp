#pragma once
#include "../include/entt.hpp"
#include "message_system.hpp"
#include "scene.hpp"

class MainMenuScene final : public Scene {
public:
    MainMenuScene();
    ~MainMenuScene() = default;

    void proccessEvents() override;
    void update() override;
    void draw() const override;
private:
    static void playCallback(entt::any data)
    { 
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::PLAY,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }
    static void settingsCallback(entt::any data)
    {
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::SETTINGS,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }
    static void exitCallback(entt::any data)
    {
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::EXIT,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }
};
