#include "hangar_scene.hpp"
#include "aseprite.hpp"
#include "background_component.hpp"
#include "button_system.hpp"
#include "fade_component.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "graphics_system.hpp"
#include "resource_system.hpp"
#include "widget_components.hpp"
#include "../include/raygui.h"
#include "widget_system.hpp"
#include <fstream>

HangarScene::HangarScene()
{
    BackgroundComponent::create(object_registry, ResourceSystem::getTexture("menu_background"), -30.f);
    fade_in = object_registry.create();
    fade_out = object_registry.create();

    object_registry.emplace<FadeEffect::Component>(fade_out, FadeEffect::create(0.8f, FadeEffect::Type::FADE_OUT));
    object_registry.emplace<FadeEffect::Component>(fade_in, FadeEffect::create(0.4f, FadeEffect::Type::FADE_IN, true));

    current_weapon = object_registry.create();
    current_engine = object_registry.create();
    readSave();
    initSprites();
    initWidgets();
    initShip(); 
}

void HangarScene::proccessEvents()
{
    ButtonSystem::proccessEvents(object_registry);
}

void HangarScene::update()
{
    WidgetSystem::update(object_registry);
    Aseprite::UpdateAsepriteTag(&engines[current_engine_index].tag);
    GraphicsSystem::update(object_registry); 
}

void HangarScene::draw() const
{
    Graphics::beginRender();
        GraphicsSystem::draw(object_registry);
        drawSprites();
    Graphics::endRender();
}

void HangarScene::initWidgets()
{
    const Vector2 resolution = Graphics::getCurrentResolution();
    const float panel_width = resolution.x / 4.f;
    const float panel_height = resolution.y / 2.f;
    const float panel_x = resolution.x - panel_width;
    const float panel_y = resolution.y / 2.f - panel_height / 2.f;
    const Rectangle panel_rect = {panel_x, panel_y, panel_width, panel_height};
    initStoreButtonsAndSpriteRecs(panel_rect);
    initButtons(panel_rect);
    
    WidgetComponents::createPanel(object_registry, panel_rect);
}

void HangarScene::initStoreButtonsAndSpriteRecs(const Rectangle panel_rect)
{
    const Vector2 resolution = Graphics::getCurrentResolution();
    const float arrow_button_width = resolution.x / 40.f;
    const float arrow_button_height = resolution.y / 40.f; 
    const float texture_width = resolution.x / 10.f;
    const float texture_height = resolution.y / 10.f;

    Rectangle button_rect = {
        .x = panel_rect.x + arrow_button_width * 0.3f,
        .y = panel_rect.y + arrow_button_height * 1.5f,
        .width = arrow_button_width,
        .height = arrow_button_height
    };

    weapon_rect = {
        .x = button_rect.x + texture_width / 6.f,
        .y = panel_rect.y + texture_height / 8.f,
        .width = texture_width,
        .height = texture_height
    };

    // Weapon spinner
    const auto weapon_arrow_button_left = WidgetComponents::createButton(object_registry, button_rect, nullptr, ICON_ARROW_LEFT_FILL);
    button_rect.x += panel_rect.x / 7.f;
    const auto weapon_arrow_button_right = WidgetComponents::createButton(object_registry, button_rect, nullptr, ICON_ARROW_RIGHT_FILL);

    // Engine spinner
    button_rect.x = panel_rect.x + arrow_button_width * 0.3f,
    button_rect.y += panel_rect.y / 3.f;
    engine_rect = {
        .x = button_rect.x + texture_width / 6.f,
        .y = panel_rect.y + texture_height / 1.5f,
        .width = texture_width,
        .height = texture_height
    };

    const auto engine_arrow_button_left = WidgetComponents::createButton(object_registry, button_rect, nullptr, ICON_ARROW_LEFT_FILL);
    button_rect.x += panel_rect.x / 7.f;
    const auto engine_arrow_button_right = WidgetComponents::createButton(object_registry, button_rect, nullptr, ICON_ARROW_RIGHT_FILL);
    
    // Label 
    score_text = std::make_unique<std::string>(TextFormat("Your score is: %d", score));

    button_rect.x = panel_rect.x + arrow_button_width * 0.3f;
    button_rect.y += panel_rect.y / 3.f;
    button_rect.width = MeasureText(score_text->c_str(), Graphics::getCurrentFontSize());
    WidgetComponents::createLabel(object_registry, button_rect, score_text->c_str());

    // Sprite rects
    object_registry.emplace<Rectangle>(current_weapon, weapon_rect);
    object_registry.emplace<Rectangle>(current_engine, engine_rect);

    // Callbacks
    object_registry.emplace<WidgetComponents::WidgetCallback>(weapon_arrow_button_left, leftWeaponButtonCallback, this);
    object_registry.emplace<WidgetComponents::WidgetCallback>(weapon_arrow_button_right, rightWeaponButtonCallback, this);

    object_registry.emplace<WidgetComponents::WidgetCallback>(engine_arrow_button_left, leftEngineButtonCallback, this);
    object_registry.emplace<WidgetComponents::WidgetCallback>(engine_arrow_button_right, rightEngineButtonCallback, this);
}

void HangarScene::initButtons(const Rectangle panel_rect)
{
    const Vector2 resolution = Graphics::getCurrentResolution();
    const char *start_button_text = "Start"; 
    const float start_button_width = resolution.x / 10.f;
    const float start_button_height = resolution.y / 20.f;
    const Rectangle button_rect = {
        .x = panel_rect.x + panel_rect.width / 2.f - start_button_width / 2.f,
        .y = panel_rect.y + panel_rect.height - start_button_height * 1.2f,
        .width = start_button_width,
        .height = start_button_height
    };
    const auto start_button = WidgetComponents::createButton(object_registry, button_rect, start_button_text);
    object_registry.emplace<WidgetComponents::WidgetCallback>(start_button, startCallback, nullptr);
}

void HangarScene::initSprites()
{
    weapons[0] = GraphicsComponent::createSprite("auto_cannon");
    weapons[1] = GraphicsComponent::createSprite("zapper");
    weapons[2] = GraphicsComponent::createSprite("big_space_gun");

    engines[0] = GraphicsComponent::createAnimation("base_engine", 0);
    engines[1] = GraphicsComponent::createAnimation("big_pulse_engine", 0);
    engines[2] = GraphicsComponent::createAnimation("burst_engine", 0);
    engines[3] = GraphicsComponent::createAnimation("supercharged_engine", 0);

    ship_sprite = GraphicsComponent::createSprite("ship");
}

void HangarScene::initShip()
{
    const Vector2 resolution = Graphics::getCurrentResolution();
    const float size = resolution.x / 10.f;
    const Rectangle ship_rect = {
        .x = resolution.x / 2.f - size / 2.f,
        .y = resolution.y / 2.f - size / 2.f,
        .width = size,
        .height = size 
    };

    object_registry.emplace<GraphicsComponent::Sprite>(ship, ship_sprite);
    object_registry.emplace<Rectangle>(ship, ship_rect);
}

void HangarScene::drawSprites() const
{
    const auto &ship_sprite = object_registry.get<GraphicsComponent::Sprite>(ship);
    const auto &ship_rect = object_registry.get<Rectangle>(ship);

    Aseprite::DrawAsepritePro(weapons[current_weapon_index].sprite, 0, weapon_rect, {0.f, 0.f}, 0.f, WHITE);
    Aseprite::DrawAsepriteTagPro(engines[current_engine_index].tag, engine_rect, {0.f, 0.f}, 0.f, WHITE);
    
    // Draw ship
    Aseprite::DrawAsepritePro(weapons[current_weapon_index].sprite, 0, ship_rect, {0.f, 0.f}, 0.f, WHITE);
    Aseprite::DrawAsepriteTagPro(engines[current_engine_index].tag, ship_rect, {0.f, 0.f}, 0.f, WHITE);
    Aseprite::DrawAsepritePro(ship_sprite.sprite, 0, ship_rect, {0.f, 0.f}, 0.f, WHITE);
}

void HangarScene::readSave()
{
    std::ifstream input("saves/save.data");
    if (input.is_open()) {
        input.read(reinterpret_cast<char*>(&score), sizeof(unsigned int)); // read score
        input.read(reinterpret_cast<char*>(&current_weapon_index), sizeof(unsigned int)); // read current engine 
        input.read(reinterpret_cast<char*>(&current_engine_index), sizeof(unsigned int)); // read current weapon 
    }
}
