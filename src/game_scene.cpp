#include "game_scene.hpp"
#include "background_component.hpp"
#include "clean_up_system.hpp"
#include "fade_component.hpp"
#include "game_master_components.hpp"
#include "graphics.hpp" 
#include "graphics_system.hpp"
#include "input_system.hpp"
#include "message_system.hpp"
#include "player_entity.hpp"
#include "object_system.hpp"
#include "resource_system.hpp"
#include "physics_system.hpp"
#include "game_master_entity.hpp"
#include "widget_components.hpp"
#include "widget_system.hpp"
#include "fuel_bar.hpp"

GameScene::GameScene()
{
    save = SaveSystem::load("saves/save.data");
    initGameObjects();
    initPauseWidgets();
    initGameOverWidgets();

    BackgroundComponent::create(object_registry, ResourceSystem::getTexture("battle_background"), -50.f);
    bg_music = ResourceSystem::getMusic("bg_music");
    SeekMusicStream(bg_music, 0.f);
    PlayMusicStream(bg_music);
    
    fade_in = object_registry.create();
    fade_out = object_registry.create();
    object_registry.emplace<FadeEffect::Component>(fade_out, FadeEffect::create(0.4f, FadeEffect::Type::FADE_OUT));
    object_registry.emplace<FadeEffect::Component>(fade_in, FadeEffect::create(0.4f, FadeEffect::Type::FADE_IN, true));
}

void GameScene::proccessEvents()
{
    MessageSystem::updateEntites(object_registry);
    if (is_received) {
        const auto msg = entt::any_cast<MessageSystem::PlaySceneMessage>(received_msg.msg);
        switch(msg) {
        case MessageSystem::PlaySceneMessage::PLAYER_DIED:
            current_state = State::GAME_OVER;
            saveGame();
            break; 
        default:
            break;
        }
        is_received = false;
    }
    switch(current_state) {
    case State::GAME:
        proccessGame();
        break;
    case State::PAUSE:
        proccessPause();
        break;
    case State::GAME_OVER:
        proccessGameOver();
        break;
    }
}

void GameScene::update()
{
    switch(current_state) {
    case State::GAME:
        updateGame();
        break;
    case State::PAUSE:
        updatePause();
        break;
    case State::GAME_OVER:
        updateGameOver();
        break;
    }
    UpdateMusicStream(bg_music);
}

void GameScene::draw() const 
{
    Graphics::beginRender();
        switch(current_state) {
        case State::GAME:
            drawGame();
            break;
        case State::PAUSE:
            drawPause();
            break;
        case State::GAME_OVER:
            drawGameOver();
            break;
        }
    Graphics::endRender();
}

void GameScene::proccessPause()
{

    if (FadeEffect::isDone(object_registry.get<FadeEffect::Component>(fade_in))) {
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::HANGAR,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        PlaySound(ResourceSystem::getSound("pause_out"));
        ResumeMusicStream(bg_music);
        current_state = State::GAME;
    }
    WidgetSystem::proccessEvents(pause_registry);
}

void GameScene::updatePause()
{
    WidgetSystem::update(pause_registry);
    auto view = object_registry.view<FadeEffect::Component>();
    for (auto [entity, fade] : view.each()) {
        if (!fade.paused)
            fade.lifetime -= GetFrameTime(); 
    }
}

void GameScene::drawPause() const
{
    GraphicsSystem::draw(object_registry);
    GraphicsSystem::draw(pause_registry);
    const auto &view = object_registry.view<FadeEffect::Component>();
    for (auto [entity, fade] : view.each()) {
        if (!fade.paused) {
            const Vector2 resolution = Graphics::getCurrentResolution();
            const Rectangle rect = { 0.f, 0.f, resolution.x, resolution.y };
            float alpha = 0.f; 
            if (fade.type == FadeEffect::Type::FADE_OUT)
                alpha = fade.lifetime / 1.f;
            else
                alpha = 1.f - (fade.lifetime / 1.f);
            DrawRectanglePro(rect, {0.f, 0.f}, 0.f, Fade(BLACK, alpha));
        }
    }
}

void GameScene::proccessGame()
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        PauseMusicStream(bg_music);
        PlaySound(ResourceSystem::getSound("pause_in"));
        current_state = State::PAUSE;
    }
    InputSystem::proccessEvents(object_registry);
    ObjectSystem::proccessEvents(object_registry);
}

void GameScene::updateGame()
{
    CleanUpSystem::clean(object_registry);
    PhysicsSystem::update(object_registry);    
    ObjectSystem::update(object_registry);
    GraphicsSystem::update(object_registry);
}

void GameScene::drawGame() const
{
    GraphicsSystem::draw(object_registry);
}

void GameScene::proccessGameOver()
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::HANGAR,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }
    if (FadeEffect::isDone(object_registry.get<FadeEffect::Component>(fade_in))) {
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::HANGAR,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }
    WidgetSystem::proccessEvents(game_over_registry);
}

void GameScene::updateGameOver()
{
    auto view = object_registry.view<FadeEffect::Component>();
    for (auto [entity, fade] : view.each()) 
        if (!fade.paused)
            fade.lifetime -= GetFrameTime(); 
    WidgetSystem::update(game_over_registry);
}

void GameScene::drawGameOver() const
{
    GraphicsSystem::draw(object_registry);
    GraphicsSystem::draw(game_over_registry);
    const auto &view = object_registry.view<FadeEffect::Component>();
    for (auto [entity, fade] : view.each()) {
        if (!fade.paused) {
            const Vector2 resolution = Graphics::getCurrentResolution();
            const Rectangle rect = { 0.f, 0.f, resolution.x, resolution.y };
            float alpha = 0.f; 
            if (fade.type == FadeEffect::Type::FADE_OUT)
                alpha = fade.lifetime / 1.f;
            else
                alpha = 1.f - (fade.lifetime / 1.f);
            DrawRectanglePro(rect, {0.f, 0.f}, 0.f, Fade(BLACK, alpha));
        }
    }
}

void GameScene::initGameObjects()
{
    const Vector2 resolution = Graphics::getCurrentResolution();
    const Rectangle score_rect = {resolution.x / 2.f, 0.f, resolution.x / 2.f, resolution.y / 10.f};
    const float size = resolution.x / 10.f;
    const float fuel_bar_height = resolution.y / 6.f;
    const Rectangle rect = {resolution.x / 2.f, resolution.y / 1.2f, size, size};
    const Rectangle fuel_bar_rect = {resolution.x - resolution.x / 10.f, resolution.y - fuel_bar_height * 1.2f, 50.f, fuel_bar_height};
     
    player = PlayerEntity::create(object_registry, rect, save->equiped_weapon, save->equiped_engine);
    game_master = GameMasterEntity::create(object_registry, player);
    
    WidgetComponents::createScoreLabel(object_registry, score_rect); 
    FuelBar::create(object_registry, fuel_bar_rect);
}

void GameScene::initPauseWidgets()
{
    using WidgetComponents::WidgetCallback;
    const Vector2 resolution = Graphics::getCurrentResolution();
    const float button_width = resolution.x / 10.f;
    const float button_height = resolution.y / 20.f;
    const float initial_x = resolution.x / 2 - button_width / 2;
    const char *continue_label = "Continue";
    const char *back_to_menu = "Main menu";
    const char *back_to_hangar = "Back to hangar"; 
    
    // Continue button
    float initial_y = resolution.y / 3 - button_height;
    Rectangle button_rect = {initial_x, initial_y, button_width, button_height};
    auto entity = WidgetComponents::createButton(pause_registry, button_rect, continue_label);
    pause_registry.emplace<WidgetCallback>(entity, continueCallback, this);
    initial_y += button_height * 2;

    // Hangar button
    button_rect = {initial_x, initial_y, button_width, button_height};
    entity = WidgetComponents::createButton(pause_registry, button_rect, back_to_hangar);
    pause_registry.emplace<WidgetCallback>(entity, backToHangarCallback, this);
    initial_y += button_height * 2;
    
    // Back to menu button
    button_rect = {initial_x, initial_y, button_width, button_height};
    entity = WidgetComponents::createButton(pause_registry, button_rect, back_to_menu);
    pause_registry.emplace<WidgetCallback>(entity, exitCallback, nullptr);
}

void GameScene::initGameOverWidgets()
{
    using WidgetComponents::WidgetCallback;
    const Vector2 resolution = Graphics::getCurrentResolution();
    const float button_width = resolution.x / 10.f;
    const float button_height = resolution.y / 20.f;
    const float initial_x = resolution.x / 2 - button_width / 2;
    const char *continue_label = "Restart";
    const char *back_to_menu = "Main menu";
    const char *back_to_hangar = "Back to hangar"; 

    float initial_y = resolution.y / 3 - button_height;
    
    // Reset 
    Rectangle button_rect = {initial_x, initial_y, button_width, button_height};
    auto entity = WidgetComponents::createButton(game_over_registry, button_rect, continue_label);
    game_over_registry.emplace<WidgetCallback>(entity, restartCallback, nullptr);
    initial_y += button_height * 2;
    
    // Hangar button
    button_rect = {initial_x, initial_y, button_width, button_height};
    entity = WidgetComponents::createButton(game_over_registry, button_rect, back_to_hangar);
    game_over_registry.emplace<WidgetCallback>(entity, backToHangarCallback, this);
    initial_y += button_height * 2;

    // Back to menu
    button_rect = {initial_x, initial_y, button_width, button_height};
    entity = WidgetComponents::createButton(game_over_registry, button_rect, back_to_menu);
    game_over_registry.emplace<WidgetCallback>(entity, exitCallback, nullptr);
}

void GameScene::saveGame()
{
    const char *path = "saves/save.data";
    save = SaveSystem::load(path);
    if (save.has_value()) {
        const auto &game_info = object_registry.get<GameMasterComponent::GameInfo>(game_master);
        save->score += game_info.score;        
        save->equiped_weapon = game_info.player_weapon;
        save->equiped_engine = game_info.player_engine;
        SaveSystem::save(save.value(), path);
    } else {
        const auto &game_info = object_registry.get<GameMasterComponent::GameInfo>(game_master);
        SaveSystem::Save save = {
            .score = game_info.score,
            .equiped_weapon = game_info.player_weapon,
            .equiped_engine = game_info.player_engine,
        };
        SaveSystem::save(save, path);
    }
}
