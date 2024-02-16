#pragma once
#include <memory>
#include <variant>
#include "../include/raylib.h"
#include "graphics_component.hpp"
#include "scene.hpp"
#include "weapon_entity.hpp"
#include "engine_entity.hpp"
#include "save_system.hpp"

class HangarScene : public Scene {
public:
    HangarScene();
    ~HangarScene() = default;

    void proccessEvents() override;
    void update() override;
    void draw() const override;
private:
    struct ShopItem {
        using Type = std::variant<WeaponEntity::PlayerType, EngineEntity::PlayerType>;
        std::variant<GraphicsComponent::Sprite, GraphicsComponent::Animation> graphic_item {};
        Type type;
        unsigned int price = 0;
        bool unclocked = false; 
        ShopItem(const std::string_view &key, const GraphicsComponent::RenderType type, const unsigned int price, const bool unclocked, Type item_type)
        {
            const int initial_tag = 0;
            this->price = price;
            this->unclocked = unclocked;
            this->type = item_type;

            switch(type) {
            case GraphicsComponent::RenderType::SPRITE:
                this->graphic_item = GraphicsComponent::createSprite(key);
                break;
            case GraphicsComponent::RenderType::ANIMATION:
                this->graphic_item = GraphicsComponent::createAnimation(key, initial_tag);
                break;
            default:
                break;
            }
        }
    };
    
    void saveData();
    void readSave();
    void initStoreButtonsAndSpriteRecs(const Rectangle panel_rect);
    void initButtons(const Rectangle panel_rect);
    void initWidgets();    
    void initShip();
    void initShopItems();

    void drawShopItems() const; // Custom draw function
                              //
    static void leftWeaponButtonCallback(entt::any data)
    {
        auto *scene = entt::any_cast<HangarScene*>(data);
        if (scene->current_weapon_index > 0)
            --scene->current_weapon_index;
        else
            scene->current_weapon_index = scene->weapons.size() - 1;
        if (scene->weapons[scene->current_weapon_index]->unclocked)
            scene->choosen_weapon = static_cast<WeaponEntity::PlayerType>(scene->current_weapon_index);
    }

    static void rightWeaponButtonCallback(entt::any data)
    {
        auto *scene = entt::any_cast<HangarScene*>(data);
        if (scene->current_weapon_index < scene->weapons.size() - 1)
            ++scene->current_weapon_index;
        else
            scene->current_weapon_index = 0;
        scene->choosen_weapon = static_cast<WeaponEntity::PlayerType>(scene->current_weapon_index);
        if (scene->weapons[scene->current_weapon_index]->unclocked)
            scene->choosen_weapon = static_cast<WeaponEntity::PlayerType>(scene->current_weapon_index);
    }

    static void leftEngineButtonCallback(entt::any data)
    {
        auto *scene = entt::any_cast<HangarScene*>(data);
        if (scene->current_engine_index > 0)
            --scene->current_engine_index;
        else
            scene->current_engine_index = scene->engines.size() - 1;
        if (scene->engines[scene->current_engine_index]->unclocked)
            scene->choosen_engine = static_cast<EngineEntity::PlayerType>(scene->current_engine_index);
    }

    static void rightEngineButtonCallback(entt::any data)
    {
        auto *scene = entt::any_cast<HangarScene*>(data);
        if (scene->current_engine_index < scene->engines.size() - 1)
            ++scene->current_engine_index;
        else
            scene->current_engine_index = 0;
        if (scene->engines[scene->current_engine_index]->unclocked)
            scene->choosen_engine = static_cast<EngineEntity::PlayerType>(scene->current_engine_index);
    }

    static void startCallback(entt::any data)
    {
        auto *scene = entt::any_cast<HangarScene*>(data);
        scene->saveData();
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::PLAY,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }

    static void backToMainMenuCallback(entt::any data)
    {
        auto *scene = entt::any_cast<HangarScene*>(data);
        scene->saveData();
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::MENU,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }

    static void buyWeaponCallback(entt::any data);
    static void buyEngineCallback(entt::any data);
private:
    std::optional<SaveSystem::Save> save {};

    std::array<std::unique_ptr<ShopItem>, 3> weapons{};
    std::array<std::unique_ptr<ShopItem>, 4> engines{};

    std::unique_ptr<std::string> score_text {};

    GraphicsComponent::Sprite ship_sprite {};    

    entt::entity ship = entt::null;
    entt::entity fade_in = entt::null;
    entt::entity fade_out = entt::null;
    entt::entity fade_in_menu = entt::null;
    entt::entity score_info_label = entt::null;
    size_t current_weapon_index = 0;
    size_t current_engine_index = 0;

    Rectangle weapon_rect {};
    Rectangle engine_rect {};

    Rectangle weapon_price_rect {};
    Rectangle engine_price_rect {};

    int score = 0;
    WeaponEntity::PlayerType choosen_weapon = WeaponEntity::PlayerType::AUTO_CANNON;
    EngineEntity::PlayerType choosen_engine = EngineEntity::PlayerType::BASIC;  
};
