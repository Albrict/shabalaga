#include "hangar_scene.hpp"
#include "aseprite.hpp"
#include "background_component.hpp"
#include "button_system.hpp"
#include "engine_entity.hpp"
#include "fade_component.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "graphics_system.hpp"
#include "resource_system.hpp"
#include "widget_components.hpp"
#include "../include/raygui.h"
#include "widget_system.hpp"

HangarScene::HangarScene()
{
    BackgroundComponent::create(object_registry, ResourceSystem::getTexture("menu_background"), -30.f);
    fade_in = object_registry.create();
    fade_out = object_registry.create();
    fade_in_menu = object_registry.create();

    object_registry.emplace<FadeEffect::Component>(fade_out, FadeEffect::create(0.4f, FadeEffect::Type::FADE_OUT));
    
    readSave();
    initShopItems();
    initWidgets();
    initShip(); 
}

void HangarScene::proccessEvents()
{
    if (IsKeyPressed(KEY_ESCAPE)) {
        saveData();
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::MENU,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }
    ButtonSystem::proccessEvents(object_registry);
}

void HangarScene::update()
{
    const auto &engine = &std::get<GraphicsComponent::Animation>(engines[current_engine_index]->graphic_item).tag;
    WidgetSystem::update(object_registry);
    Aseprite::UpdateAsepriteTag(engine);
    GraphicsSystem::update(object_registry); 
}

void HangarScene::draw() const
{
    Graphics::beginRender();
        GraphicsSystem::draw(object_registry);
        drawShopItems();
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
    const float buy_button_width = resolution.x / 15.f;
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

    Rectangle buy_button_rect = {
        .x = panel_rect.x + panel_rect.width - buy_button_width * 1.5f,
        .y = 0.f,
        .width = buy_button_width,
        .height = arrow_button_height
    };

    // Weapon spinner
    const auto weapon_arrow_button_left = WidgetComponents::createButton(object_registry, button_rect, nullptr, ICON_ARROW_LEFT_FILL);
    button_rect.x += panel_rect.x / 7.f;
    const auto weapon_arrow_button_right = WidgetComponents::createButton(object_registry, button_rect, nullptr, ICON_ARROW_RIGHT_FILL);
    
    buy_button_rect.y = button_rect.y;
    const auto buy_weapon_button = WidgetComponents::createButton(object_registry, buy_button_rect, "Buy");
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
    buy_button_rect.y = button_rect.y;
    const auto buy_engine_button = WidgetComponents::createButton(object_registry, buy_button_rect, "Buy");
    
    // Label 
    score_text = std::make_unique<std::string>(TextFormat("Your score is: %d", score));

    button_rect.x = panel_rect.x + arrow_button_width * 0.3f;
    button_rect.y += panel_rect.y / 3.f;
    button_rect.width = MeasureText(score_text->c_str(), Graphics::getCurrentFontSize());
    WidgetComponents::createLabel(object_registry, button_rect, score_text->c_str());

    // Callbacks
    object_registry.emplace<WidgetComponents::WidgetCallback>(weapon_arrow_button_left, leftWeaponButtonCallback, this);
    object_registry.emplace<WidgetComponents::WidgetCallback>(weapon_arrow_button_right, rightWeaponButtonCallback, this);

    object_registry.emplace<WidgetComponents::WidgetCallback>(engine_arrow_button_left, leftEngineButtonCallback, this);
    object_registry.emplace<WidgetComponents::WidgetCallback>(engine_arrow_button_right, rightEngineButtonCallback, this);
    
    object_registry.emplace<WidgetComponents::WidgetCallback>(buy_weapon_button, buyWeaponCallback, this);
    object_registry.emplace<WidgetComponents::WidgetCallback>(buy_engine_button, buyEngineCallback, this);

    weapon_price_rect = weapon_rect;
    engine_price_rect = engine_rect;

    weapon_price_rect.width /= 2.f;
    weapon_price_rect.height /= 1.5f;
    weapon_price_rect.x += weapon_price_rect.width / 2.f;

    engine_price_rect.width /= 2.f;
    engine_price_rect.height /= 2.f;
    engine_price_rect.x += engine_price_rect.width / 2.f;
    engine_price_rect.y += engine_price_rect.height / 1.5f;
}

void HangarScene::initButtons(const Rectangle panel_rect)
{
    const Vector2 resolution = Graphics::getCurrentResolution();
    const char *start_button_text = "Start"; 
    const char *back_to_main_menu_button_text = "Back to main menu"; 
    const float button_width = resolution.x / 10.f;
    const float button_height = resolution.y / 20.f;
    const Rectangle start_button_rect = {
        .x = panel_rect.x + panel_rect.width / 2.f - button_width / 2.f,
        .y = panel_rect.y + panel_rect.height - button_height * 1.2f,
        .width =  button_width,
        .height = button_height
    };
    
    const Rectangle back_to_menu_rect = {
        .x = button_width / 2.f,
        .y = button_height,
        .width = button_width * 1.5f,
        .height = button_height
    };

    const auto start_button = WidgetComponents::createButton(object_registry, start_button_rect, start_button_text);
    object_registry.emplace<WidgetComponents::WidgetCallback>(start_button, startCallback, this);

    const auto back_to_menu_button = WidgetComponents::createButton(object_registry, back_to_menu_rect, back_to_main_menu_button_text);
    object_registry.emplace<WidgetComponents::WidgetCallback>(back_to_menu_button, backToMainMenuCallback, this);
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

void HangarScene::drawShopItems() const
{
    const auto &ship_sprite = object_registry.get<GraphicsComponent::Sprite>(ship);
    const auto &ship_rect = object_registry.get<Rectangle>(ship);
    const auto weapon_sprite = std::get<GraphicsComponent::Sprite>(weapons[current_weapon_index]->graphic_item).sprite;
    const auto engine_animation = std::get<GraphicsComponent::Animation>(engines[current_engine_index]->graphic_item).tag;
     
    Aseprite::DrawAsepritePro(weapon_sprite, 0, weapon_rect, {0.f, 0.f}, 0.f, WHITE);
    Aseprite::DrawAsepriteTagPro(engine_animation, engine_rect, {0.f, 0.f}, 0.f, WHITE);
    
    
    if (!weapons[current_weapon_index]->unclocked) {
        const float text_size = MeasureText(TextFormat("%d", weapons[current_weapon_index]->price), Graphics::getCurrentFontSize());
        Color text_color = WHITE;
        Rectangle text_rect = weapon_price_rect;
        DrawRectangleRec(weapon_price_rect, ColorAlpha(BLACK, 0.6f));
        text_rect.x += text_rect.width / 2.f - text_size / 2.f;
        text_rect.y += text_rect.height / 2.f - text_size / 2.5f;

        if (weapons[current_weapon_index]->price <= score)
            text_color = GREEN;
        else
            text_color = RED;
        DrawText(TextFormat("%d", weapons[current_weapon_index]->price), 
                text_rect.x, text_rect.y,Graphics::getCurrentFontSize(), text_color);
    }
    
    if (!engines[current_engine_index]->unclocked) {
        const float text_size = MeasureText(TextFormat("%d", engines[current_engine_index]->price), Graphics::getCurrentFontSize());
        Color text_color = WHITE;
        Rectangle text_rect = engine_price_rect;
        DrawRectangleRec(engine_price_rect, ColorAlpha(BLACK, 0.6f));
        text_rect.x += text_rect.width / 2.f - text_size / 2.f; 
        text_rect.y += text_rect.height / 2.f - text_size / 2.5f;

        if (engines[current_engine_index]->price <= score)
            text_color = GREEN;
        else
            text_color = RED;
        DrawText(TextFormat("%d", engines[current_engine_index]->price), 
                text_rect.x, text_rect.y, Graphics::getCurrentFontSize(), text_color);
    }

    // Draw ship
    Aseprite::DrawAsepritePro(weapon_sprite, 0, ship_rect, {0.f, 0.f}, 0.f, WHITE);
    Aseprite::DrawAsepriteTagPro(engine_animation, ship_rect, {0.f, 0.f}, 0.f, WHITE);
    Aseprite::DrawAsepritePro(ship_sprite.sprite, 0, ship_rect, {0.f, 0.f}, 0.f, WHITE);
}

void HangarScene::initShopItems()
{
    if (save.has_value()) {
        // Weapons
        weapons[0] = std::make_unique<ShopItem>("auto_cannon", GraphicsComponent::RenderType::SPRITE, 0, true, 
                                                WeaponEntity::PlayerType::AUTO_CANNON);
        weapons[1] = std::make_unique<ShopItem>("zapper", GraphicsComponent::RenderType::SPRITE, 4000, save->weapons[1], 
                                                WeaponEntity::PlayerType::ZAPPER);
        weapons[2] = std::make_unique<ShopItem>("big_space_gun", GraphicsComponent::RenderType::SPRITE, 12000, save->weapons[2], 
                                                WeaponEntity::PlayerType::BIG_SPACE_GUN);
        // Engines
        engines[0] = std::make_unique<ShopItem>("base_engine", GraphicsComponent::RenderType::ANIMATION, 0, true, EngineEntity::PlayerType::BASIC);
        engines[1] = std::make_unique<ShopItem>("big_pulse_engine", GraphicsComponent::RenderType::ANIMATION, 6000, save->engines[1], 
                                                EngineEntity::PlayerType::BIG_PULSE_ENGINE);
        engines[2] = std::make_unique<ShopItem>("burst_engine", GraphicsComponent::RenderType::ANIMATION, 8000, save->engines[2], 
                                                EngineEntity::PlayerType::BURST_ENGINE);
        engines[3] = std::make_unique<ShopItem>("supercharged_engine", GraphicsComponent::RenderType::ANIMATION, 12000, save->engines[3], 
                                            EngineEntity::PlayerType::SUPERCHARGED_ENGINE);
    } else {
        weapons[0] = std::make_unique<ShopItem>("auto_cannon", GraphicsComponent::RenderType::SPRITE, 0, true, WeaponEntity::PlayerType::AUTO_CANNON);
        weapons[1] = std::make_unique<ShopItem>("zapper", GraphicsComponent::RenderType::SPRITE, 4000, false, WeaponEntity::PlayerType::ZAPPER);
        weapons[2] = std::make_unique<ShopItem>("big_space_gun", GraphicsComponent::RenderType::SPRITE, 12000, false, WeaponEntity::BIG_SPACE_GUN);

        engines[0] = std::make_unique<ShopItem>("base_engine", GraphicsComponent::RenderType::ANIMATION, 0, true, EngineEntity::PlayerType::BASIC);
        engines[1] = std::make_unique<ShopItem>("big_pulse_engine", GraphicsComponent::RenderType::ANIMATION, 6000, false, 
                                                EngineEntity::PlayerType::BIG_PULSE_ENGINE);
        engines[2] = std::make_unique<ShopItem>("burst_engine", GraphicsComponent::RenderType::ANIMATION, 8000, false, 
                                                EngineEntity::PlayerType::BURST_ENGINE);
        engines[3] = std::make_unique<ShopItem>("supercharged_engine", GraphicsComponent::RenderType::ANIMATION, 12000, false, 
                                                EngineEntity::PlayerType::SUPERCHARGED_ENGINE);

    }
    ship_sprite = GraphicsComponent::createSprite("ship");
}

void HangarScene::saveData()
{
    SaveSystem::Save save = {
        .score = score,
        .equiped_weapon = choosen_weapon,
        .equiped_engine = choosen_engine
    };

    for (const auto &weapon : weapons)
        save.weapons.push_back(weapon->unclocked);
    for (const auto &engine : engines)
        save.engines.push_back(engine->unclocked);

    SaveSystem::save(save, "saves/save.data");
}

void HangarScene::readSave()
{
    save = SaveSystem::load("saves/save.data"); 
    if (save.has_value()) {
        score = save->score;
        current_weapon_index = save->equiped_weapon;
        current_engine_index = save->equiped_engine;
        choosen_weapon = save->equiped_weapon;
        choosen_engine = save->equiped_engine;
    }
}

void HangarScene::buyWeaponCallback(entt::any data)
{
    auto *scene = entt::any_cast<HangarScene*>(data);
    const unsigned int weapon_price = scene->weapons[scene->current_weapon_index]->price;
    if (scene->score < weapon_price || scene->weapons[scene->current_weapon_index]->unclocked) {
        return;
    } else {
        scene->score -= weapon_price;
        *scene->score_text = TextFormat("Your score is: %d", scene->score);
        scene->weapons[scene->current_weapon_index]->unclocked = true;
        scene->choosen_weapon = static_cast<WeaponEntity::PlayerType>(scene->current_weapon_index);
        scene->saveData();
    }
}

void HangarScene::buyEngineCallback(entt::any data)
{
    auto *scene = entt::any_cast<HangarScene*>(data);
    const unsigned int engine_price = scene->engines[scene->current_engine_index]->price;
    if (scene->score < engine_price || scene->engines[scene->current_engine_index]->unclocked) {
        return;
    } else {
        scene->score -= engine_price;
        *scene->score_text = TextFormat("Your score is: %d", scene->score);
        scene->engines[scene->current_engine_index]->unclocked = true;
        scene->choosen_engine = static_cast<EngineEntity::PlayerType>(scene->current_engine_index);
        scene->saveData();
    }
}
