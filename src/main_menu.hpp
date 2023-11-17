#pragma once
#include "scene.hpp"
#include "../include/entt.hpp"
#include "background_system.hpp"

class MainMenu final : public Scene {
public:
    MainMenu();
    ~MainMenu()
    { BackgroundSystem::clear(m_menu_registry); }
    void proccessEvents() override;
    void update() override;
    void draw() const override;
private:
    static void playCallback(entt::any data)
    { auto scene = entt::any_cast<MainMenu*>(data); scene->emmiter.publish(Scene::Messages::PLAY);}
    static void settingsCallback(entt::any data)
    { auto scene = entt::any_cast<MainMenu*>(data); scene->emmiter.publish(Scene::Messages::SETTINGS); }
    static void exitCallback(entt::any data)
    { auto scene = entt::any_cast<MainMenu*>(data); scene->emmiter.publish(Scene::Messages::EXIT); }
private:
    entt::registry m_menu_registry {};
};
