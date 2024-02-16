#pragma once
#include "../include/entt.hpp"
#include "fade_component.hpp"
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
        auto *scene = entt::any_cast<MainMenuScene*>(data);
        FadeEffect::resume(scene->object_registry.get<FadeEffect::Component>(scene->fade_out));
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
private:
    entt::entity fade_in = entt::null;
    entt::entity fade_out = entt::null;
};
