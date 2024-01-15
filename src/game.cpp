#include "../include/raygui.h"
#include "../include/style_cyber.h"
#include "game.hpp"
#include "graphics.hpp"
#include "main_menu.hpp"
#include "message_system.hpp"
#include "resource_system.hpp"
#include "settings_scene.hpp"
#include "game_scene.hpp"

namespace Game {
    entt::registry object_registry;
    std::unique_ptr<Scene> current_scene;
    bool running = false;
    
    void proccessMessages(MessageSystem::Message msg);
    void proccessSceneMessages(const MessageSystem::SceneMessage msg);
    void proccessGameSceneMessages(const MessageSystem::PlaySceneMessage msg);
    void proccessEvents();
    void update();
    void draw();
    void loadResources();
}

void Game::init()
{
    auto listener = std::make_unique<MessageSystem::Listener>();
    listener->cb = proccessMessages;
    MessageSystem::registrListener(std::move(listener), "game_loop");
    Graphics::init();
    InitAudioDevice();
    loadResources();
       
    current_scene = std::make_unique<MainMenuScene>();

    running = true;
    SetExitKey(KEY_NULL);
}

void Game::run()
{
    while(running) {
        if (WindowShouldClose())
            running = false;
        current_scene->proccessEvents();
        MessageSystem::update();
        current_scene->update();
        current_scene->draw();
    }
}

void Game::quit()
{
    MessageSystem::unregistrListener("game_loop");
    ResourceSystem::unloadResources();
    CloseAudioDevice();
}

void Game::loadResources()
{
    // Backgrounds
    ResourceSystem::loadTexture("assets/backgrounds/main_menu.png", "menu_background");
    ResourceSystem::loadTexture("assets/backgrounds/battle.png", "battle_background");
    
    // Player
    ResourceSystem::loadAseprite("assets/objects/ship/full_health.aseprite", "ship");
    
    // Engines
    ResourceSystem::loadAseprite("assets/objects/engines/bomber_engine.aseprite", "bomber_engine"); 
    ResourceSystem::loadAseprite("assets/objects/engines/fighter_engine.aseprite", "fighter_engine");
    ResourceSystem::loadAseprite("assets/objects/engines/base_engine_powering.aseprite", "base_engine");
    
    // Weapons
    ResourceSystem::loadAseprite("assets/objects/weapons/auto_cannon.aseprite", "auto_cannon");
    ResourceSystem::loadAseprite("assets/objects/weapons/fighter_weapon.aseprite", "fighter_weapon");
    
    // Projectiles 
    ResourceSystem::loadAseprite("assets/objects/projectiles/bullet.aseprite", "bullet_projectile");
    ResourceSystem::loadAseprite("assets/objects/projectiles/auto_cannon_projectile.aseprite", "auto_cannon_projectile");

    // Enemies 
    ResourceSystem::loadAseprite("assets/objects/enemies/bomber.aseprite", "bomber"); 
    ResourceSystem::loadAseprite("assets/objects/enemies/fighter.aseprite", "fighter");
    
    // Explosions
    ResourceSystem::loadAseprite("assets/effects/player_explosion.aseprite", "player_explosion");
    ResourceSystem::loadAseprite("assets/effects/bomber_explosion.aseprite", "bomber_explosion");
    ResourceSystem::loadAseprite("assets/effects/small_explosion.aseprite", "small_explosion");
    ResourceSystem::loadAseprite("assets/effects/fighter_explosion.aseprite", "fighter_explosion");
    
    // Pick ups 
    ResourceSystem::loadAseprite("assets/pickups/fuel.aseprite", "fuel_pickup");
    
    // Hud 
    ResourceSystem::loadAseprite("assets/ui/fuel_bar.aseprite", "fuel_bar");

    // Sounds
    ResourceSystem::loadSound("assets/sound/enemy_defeated_01.wav", "enemy_destroyed_01");
    ResourceSystem::loadSound("assets/sound/enemy_defeated_02.wav", "enemy_destroyed_02");
    ResourceSystem::loadSound("assets/sound/attack_01.wav", "attack_01");
    ResourceSystem::loadSound("assets/sound/attack_02.wav", "attack_02");
    ResourceSystem::loadSound("assets/sound/hitsound_01.wav", "hitsound_01");
    ResourceSystem::loadSound("assets/sound/hitsound_02.wav", "hitsound_02");
    ResourceSystem::loadSound("assets/sound/player_hitsound_01.wav", "player_hitsound_01");
    ResourceSystem::loadSound("assets/sound/player_hitsound_02.wav", "player_hitsound_02");
    ResourceSystem::loadSound("assets/sound/player_defeated.wav", "player_defeated");
    ResourceSystem::loadSound("assets/sound/ui_01.wav", "ui_01");
    ResourceSystem::loadSound("assets/sound/pause_in.wav", "pause_in");
    ResourceSystem::loadSound("assets/sound/pause_out.wav", "pause_out");

    // Music
    ResourceSystem::loadMusic("assets/sound/systems.mp3", "bg_music");
}

void Game::proccessMessages(MessageSystem::Message msg)
{
    switch(msg.type) {
    using enum MessageSystem::Type;
    case SCENE_MESSAGE:
        proccessSceneMessages(entt::any_cast<MessageSystem::SceneMessage>(msg.msg));
        break;
    case PLAY_SCENE_MESSAGE:
        current_scene->sendMessage(msg);
        break;
    default: 
        break;
    }
}

void Game::proccessSceneMessages(const MessageSystem::SceneMessage msg)
{
    switch(msg) {
    using enum MessageSystem::SceneMessage;
    case MENU:
        current_scene.reset(new MainMenuScene);
        break;
    case PLAY:
        current_scene.reset(new GameScene);
        break;
    case SETTINGS:
        current_scene.reset(new SettingsScene);
        break;
    case EXIT:
        running = false;
        break;
    } 
}

void Game::proccessGameSceneMessages(const MessageSystem::PlaySceneMessage msg)
{

}
