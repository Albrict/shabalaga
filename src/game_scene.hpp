#pragma once
#include "message_system.hpp"
#include "scene.hpp"
#include "../include/entt.hpp"
#include "../include/raylib.h"

class GameScene final : public Scene {
public:
    GameScene();
    ~GameScene() = default;
    
    void proccessEvents() override;
    void update() override;
    void draw() const override;
private:
    // Pause state
    void proccessPause();
    void updatePause();
    void drawPause() const;
    
    // Game state
    void proccessGame();
    void updateGame();
    void drawGame() const;

    void initGameObjects();
    void initPauseWidgets();
    void initInput();

    static void continueCallback(entt::any data)
    { 
        GameScene *scene = entt::any_cast<GameScene*>(data);
        scene->current_state = State::GAME;
    }

    static void exitCallback(entt::any data)
    {
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::MENU,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }
private:
    enum class State {
        PAUSE,
        GAME
    };
    State current_state = State::GAME;
    entt::registry pause_registry {};
    Music bg_music {};
};
