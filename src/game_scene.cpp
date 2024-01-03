#include "game_scene.hpp"
#include "background_component.hpp"
#include "clean_up_system.hpp"
#include "graphics.hpp"
#include "graphics_system.hpp"
#include "input_system.hpp"
#include "player_entity.hpp"
#include "object_system.hpp"
#include "engine_entity.hpp"
#include "resource_system.hpp"
#include "physics_system.hpp"
#include "ship_components.hpp"
#include "game_master_entity.hpp"
#include "auto_cannon_entity.hpp"
#include "widget_components.hpp"
#include "widget_system.hpp"

GameScene::GameScene()
{
    auto listener = std::make_unique<MessageSystem::Listener>();
    initGameObjects();
    initPauseWidgets();
    BackgroundComponent::create(object_registry, ResourceSystem::getTexture("battle_background"), -50.f);
}

void GameScene::proccessEvents()
{
    switch(current_state) {
    case State::GAME:
        proccessGame();
        break;
    case State::PAUSE:
        proccessPause();
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
    }
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
        }
    Graphics::endRender();
}

void GameScene::proccessPause()
{
    if (IsKeyPressed(KEY_ESCAPE))
        current_state = State::GAME;
    WidgetSystem::proccessEvents(pause_registry);
}

void GameScene::updatePause()
{
    WidgetSystem::update(pause_registry);
}

void GameScene::drawPause() const
{
    GraphicsSystem::draw(object_registry);
    GraphicsSystem::draw(pause_registry);
}

void GameScene::proccessGame()
{
    if (IsKeyPressed(KEY_ESCAPE))
        current_state = State::PAUSE;
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

void GameScene::initGameObjects()
{
    auto player = object_registry.create();
    const Vector2 resolution = Graphics::getCurrentResolution();
    const Vector2 position = {resolution.x / 2.f, resolution.y / 1.2f};
    const float width = resolution.x / 10.f;
    const Rectangle rect = {position.x, position.y, width, width};

    const std::string_view player_sprite = "ship";
    const std::string_view engine_sprite = "engine"; 
     
    player = PlayerEntity::create(object_registry, position, player_sprite, width, width);
    const auto engine = EngineEntity::create(object_registry, EngineEntity::Type::BASIC, position,width, width);
    const auto weapon = AutoCannonEntity::create(object_registry, rect);

    const auto game_master = GameMasterEntity::create(object_registry, player);
    ShipComponents::attachComponents(object_registry, player, weapon, engine);
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

    float initial_y = resolution.y / 3 - button_height;
    Rectangle button_rect = {initial_x, initial_y, button_width, button_height};
    auto entity = WidgetComponents::createButton(pause_registry, button_rect, continue_label);
    pause_registry.emplace<WidgetCallback>(entity, continueCallback, this);
    initial_y += button_height * 2;

    button_rect = {initial_x, initial_y, button_width, button_height};
    entity = WidgetComponents::createButton(pause_registry, button_rect, back_to_menu);
    pause_registry.emplace<WidgetCallback>(entity, exitCallback, nullptr);
}
