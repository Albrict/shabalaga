#pragma once
#include "fade_component.hpp"
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
    void saveGame();

    // Pause state
    void proccessPause();
    void updatePause();
    void drawPause() const;
    
    // Game state
    void proccessGame();
    void updateGame();
    void drawGame() const;
    
    // Game over state 
    void proccessGameOver();
    void updateGameOver();
    void drawGameOver() const;

    void initGameObjects();
    void initPauseWidgets();
    void initGameOverWidgets();
    void initInput();

    static void continueCallback(entt::any data)
    { 
        GameScene *scene = entt::any_cast<GameScene*>(data);
        scene->current_state = State::GAME;
        ResumeMusicStream(scene->bg_music);
    }
    
    static void backToHangarCallback(entt::any data)
    {
        auto *scene = entt::any_cast<GameScene*>(data);
        FadeEffect::resume(scene->object_registry.get<FadeEffect::Component>(scene->fade_in));
    }

    static void restartCallback(entt::any data)
    {
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::PLAY,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
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
        GAME,
        GAME_OVER
    };
    entt::entity player = entt::null;
    entt::entity game_master = entt::null;
    entt::entity fade_in = entt::null;
    entt::entity fade_out = entt::null;

    State current_state = State::GAME;
    entt::registry pause_registry {};
    entt::registry game_over_registry {};
    Music bg_music {};
};
