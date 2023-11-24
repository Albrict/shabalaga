#include "../include/raygui.h"
#include "../include/raymath.h"
#include "../include/style_cyber.h"
#include "game.hpp"
#include "graphics.hpp"
#include "main_menu.hpp"
#include "resource_system.hpp"
#include "settings_scene.hpp"
#include "game_scene.hpp"

Game *Game::game_instance = nullptr;

Game::Game() 
{
    Graphics::init();
    running = true;
    loadResources();
}

void Game::initFirstScene() 
{
    current_scene = std::make_unique<MainMenu>();
    current_scene->getEmmiter().on<Scene::Messages>(
    [this](const Scene::Messages &event, Scene::Emmiter &emmiter) {
        proccessMessages(event, emmiter);
    });
}

void Game::run() 
{
    while (running) {
        if (WindowShouldClose())
            running = false;
        proccessEvents();
        update();
        draw();
    }
}

void Game::proccessEvents() 
{
    if (change_to_scene != Scenes::NONE) {
        switch (change_to_scene) {
        case Scenes::PLAY:
            changeSceneTo(new GameScene);
            change_to_scene = Scenes::NONE;
            break;
        case Scenes::SETTINGS:
            changeSceneTo(new SettingsScene);
            change_to_scene = Scenes::NONE;
            break;
        case Scenes::MAIN_MENU:
            changeSceneTo(new MainMenu);
            change_to_scene = Scenes::NONE;
            break;
        default:
            break;
        };
    }
    current_scene->proccessEvents();
}

void Game::update() { current_scene->update(); }

void Game::draw() const 
{
    Graphics::beginRender();
        current_scene->draw();
        DrawFPS(0, 0);
    Graphics::endRender();
}

void Game::proccessMessages(const Scene::Messages &message,
                            Scene::Emmiter &emmiter)
{
    switch (message) {
    using Messages = Scene::Messages;
    case Messages::PLAY:
        change_to_scene = Scenes::PLAY;
        break;
    case Messages::SETTINGS:
        change_to_scene = Scenes::SETTINGS;
        break;
    case Messages::MAIN_MENU:
        change_to_scene = Scenes::MAIN_MENU;
        break;
    case Messages::EXIT:
        running = false;
        break;
    default:
        break;
    }
}

void Game::loadResources()
{
    ResourceSystem::loadTexture("assets/backgrounds/main_menu.png", "menu_background");
    ResourceSystem::loadSprite("assets/objects/ship/full_health.aseprite", "ship");
    ResourceSystem::loadSprite("assets/objects/engines/base_engine_powering.aseprite", "engine");
    ResourceSystem::loadSprite("assets/objects/weapons/auto_cannon.aseprite", "auto_cannon");
}
