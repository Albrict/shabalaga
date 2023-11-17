#include "../include/raygui.h"
#include "../include/raymath.h"
#include "../include/style_cyber.h"
#include "game.hpp"
#include "main_menu.hpp"
#include "settings_scene.hpp"

Game *Game::game_instance = nullptr;

Game::Game() 
{
    const int window_width = GetScreenWidth();
    const int window_height = GetScreenHeight();
     
    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    InitWindow(window_width, window_height, "Shabalaga");
    SetWindowMinSize(640, 480);
    SetTargetFPS(GetMonitorRefreshRate(0));

    GuiLoadStyleCyber();
    GuiSetStyle(DEFAULT, TEXT_SIZE, 28);

    current_resolution = {1920.f, 1200.f};
    target = LoadRenderTexture(current_resolution.x, current_resolution.y);
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
    UnloadRenderTexture(target);
}

void Game::proccessEvents() 
{
    if (change_to_scene != Scenes::NONE) {
        switch (change_to_scene) {
        case Scenes::PLAY:
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
    float scale = std::min((GetScreenWidth() / current_resolution.x),
                            (GetScreenHeight() / current_resolution.y));
    SetMouseOffset(-(GetScreenWidth() - (current_resolution.x * scale)) * 0.5f,
                    -(GetScreenHeight() - (current_resolution.y * scale)) * 0.5f);
    SetMouseScale(1 / scale, 1 / scale);

    BeginTextureMode(target);
    {
        ClearBackground(BLACK);
        current_scene->draw();
    }
    EndTextureMode();

    BeginDrawing();
    {
        ClearBackground(BLACK);
        const Rectangle source_rect = {0.0f, 0.0f,
                                    static_cast<float>(target.texture.width),
                                    static_cast<float>(-target.texture.height)};

        const Rectangle dest_rect = {
            (GetScreenWidth() - (current_resolution.x * scale)) * 0.5f,
            (GetScreenHeight() - (current_resolution.y * scale)) * 0.5f,
            current_resolution.x * scale, current_resolution.y * scale};
        DrawTexturePro(target.texture, source_rect, dest_rect, {0.f, 0.f}, 0.0f,
                    ColorBrightness(WHITE, brightness));
    }
    EndDrawing();
}

void Game::proccessMessages(const Scene::Messages &message,
                            Scene::Emmiter &emmiter)
{
    switch (message) {
    using Messages = Scene::Messages;
    case Messages::PLAY:
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
}
