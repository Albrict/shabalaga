#include "../include/raygui.h"
#include "../include/style_cyber.h"
#include "game.hpp"
#include "graphics.hpp"
#include "hangar_scene.hpp"
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
    void readSettings();
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
    readSettings();
}

void Game::run()
{
    while(running) {
        if (WindowShouldClose())
            running = false;
        MessageSystem::update();
        current_scene->proccessEvents();
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
     
    // Playe engines 
    ResourceSystem::loadAseprite("assets/objects/engines/base_engine.aseprite", "base_engine");
    ResourceSystem::loadAseprite("assets/objects/engines/big_pulse_engine.aseprite", "big_pulse_engine");
    ResourceSystem::loadAseprite("assets/objects/engines/burst_engine.aseprite", "burst_engine");
    ResourceSystem::loadAseprite("assets/objects/engines/supercharged_engine.aseprite", "supercharged_engine");

    // Enemy engines 
    ResourceSystem::loadAseprite("assets/objects/engines/bomber_engine.aseprite", "bomber_engine"); 
    ResourceSystem::loadAseprite("assets/objects/engines/fighter_engine.aseprite", "fighter_engine");
    ResourceSystem::loadAseprite("assets/objects/engines/scout_engine.aseprite", "scout_engine");    

    // Player weapons
    ResourceSystem::loadAseprite("assets/objects/weapons/auto_cannon.aseprite", "auto_cannon");
    ResourceSystem::loadAseprite("assets/objects/weapons/big_space_gun.aseprite", "big_space_gun");
    ResourceSystem::loadAseprite("assets/objects/weapons/zapper.aseprite", "zapper");
    
    // Enemy weapons
    ResourceSystem::loadAseprite("assets/objects/weapons/fighter_weapon.aseprite", "fighter_weapon");
    ResourceSystem::loadAseprite("assets/objects/weapons/scout_weapon.aseprite", "scout_weapon");
    
    // Projectiles 
    ResourceSystem::loadAseprite("assets/objects/projectiles/bullet.aseprite", "bullet_projectile");
    ResourceSystem::loadAseprite("assets/objects/projectiles/auto_cannon_projectile.aseprite", "auto_cannon_projectile");
    ResourceSystem::loadAseprite("assets/objects/projectiles/big_space_gun_projectile.aseprite", "big_space_gun_projectile");
    ResourceSystem::loadAseprite("assets/objects/projectiles/zapper_projectile.aseprite", "zapper_projectile");

    // Enemies 
    ResourceSystem::loadAseprite("assets/objects/enemies/bomber.aseprite", "bomber"); 
    ResourceSystem::loadAseprite("assets/objects/enemies/fighter.aseprite", "fighter");
    ResourceSystem::loadAseprite("assets/objects/enemies/scout.aseprite", "scout"); 
    // Explosions
    ResourceSystem::loadAseprite("assets/effects/player_explosion.aseprite", "player_explosion");
    ResourceSystem::loadAseprite("assets/effects/bomber_explosion.aseprite", "bomber_explosion");
    ResourceSystem::loadAseprite("assets/effects/fighter_explosion.aseprite", "fighter_explosion");
    ResourceSystem::loadAseprite("assets/effects/scout_explosion.aseprite", "scout_explosion");

    ResourceSystem::loadAseprite("assets/effects/small_explosion.aseprite", "small_explosion");
    ResourceSystem::loadAseprite("assets/effects/bsg_explosion.aseprite", "bsg_explosion");
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
    ResourceSystem::loadSound("assets/sound/fuel_pickup_01.wav", "fuel_pickup_01");
    ResourceSystem::loadSound("assets/sound/fuel_pickup_02.wav", "fuel_pickup_02");
    
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
    case HANGAR:
        current_scene.reset(new HangarScene);
        break;
    case SETTINGS:
        current_scene.reset(new SettingsScene);
        break;
    case EXIT:
        running = false;
        break;
    } 
}

void Game::readSettings()
{
    const auto settings = SaveSystem::loadSettings("saves/settings.data");
    if (settings.has_value()) {
        Graphics::setBrightness(settings->brightness);
        SetMasterVolume(settings->sound);
        if (settings->is_fullscreen) {
            if (!IsWindowFullscreen())
                ToggleFullscreen(); 
        } else {
            if (IsWindowFullscreen())
                ToggleFullscreen();
        }
    }
}
