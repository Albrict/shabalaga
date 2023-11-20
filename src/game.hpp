#pragma once
#include "scene.hpp"
#include "resource_system.hpp"

class Game {
public:
    Game(const Game&) = delete;
    Game& operator=(Game&) = delete;
    static Game *Instance() {
        if (game_instance) 
            return game_instance;
        else
            return game_instance = new Game();
    }
    
    void setBrightness(const float brightness_value)
    { brightness = brightness_value; }
    void initFirstScene();
    void run();
    void closeGame()
    { delete game_instance; ResourceSystem::unloadTextures(); CloseWindow(); }
private:
    Game();
    
    void changeSceneTo(Scene *new_scene)
    {
        current_scene.reset(new_scene);
        current_scene->getEmmiter().on<Scene::Messages>([this](const Scene::Messages &event, Scene::Emmiter &emmiter) {
            proccessMessages(event, emmiter);
        });
    }
    void loadResources();
    void proccessMessages(const Scene::Messages &message, Scene::Emmiter &emmiter);
    void proccessEvents();
    void update();
    void draw() const;
private:
    enum class Scenes {
        PLAY,
        SETTINGS,
        MAIN_MENU,
        NONE
    };

    static Game *game_instance;
    std::unique_ptr<Scene> current_scene {};
    bool running = false;
    Scenes change_to_scene = Scenes::NONE; 
    float brightness = 0.f;
};
