#pragma once
#include "graphics_component.hpp"
#include "scene.hpp"
#include <memory>
#include "../include/raylib.h"

class HangarScene : public Scene {
public:
    HangarScene();
    ~HangarScene() = default;

    void proccessEvents() override;
    void update() override;
    void draw() const override;
private:
    void readSave();
    void initStoreButtonsAndSpriteRecs(const Rectangle panel_rect);
    void initButtons(const Rectangle panel_rect);
    void initWidgets();    
    void initSprites();
    void initShip();

    void drawSprites() const; // Custom draw function
                              //
    static void leftWeaponButtonCallback(entt::any data)
    {
        auto *scene = entt::any_cast<HangarScene*>(data);
        if (scene->current_weapon_index > 0) {
            --scene->current_weapon_index;
        } else {
            scene->current_weapon_index = scene->weapons.size() - 1;
            return;
        }
    }

    static void rightWeaponButtonCallback(entt::any data)
    {
        auto *scene = entt::any_cast<HangarScene*>(data);
        if (scene->current_weapon_index < scene->weapons.size() - 1) {
            ++scene->current_weapon_index;
        } else {
            scene->current_weapon_index = 0;
            return;
        }
    }

    static void leftEngineButtonCallback(entt::any data)
    {
        auto *scene = entt::any_cast<HangarScene*>(data);
        if (scene->current_engine_index > 0) {
            --scene->current_engine_index;
        } else {
            scene->current_engine_index = scene->engines.size() - 1;
            return;
        }
    }

    static void rightEngineButtonCallback(entt::any data)
    {
        auto *scene = entt::any_cast<HangarScene*>(data);
        if (scene->current_engine_index < scene->engines.size() - 1) {
            ++scene->current_engine_index;
        } else {
            scene->current_engine_index = 0;
            return;
        }
    }

    static void startCallback(entt::any data)
    {
        MessageSystem::Message msg = {.msg = MessageSystem::SceneMessage::PLAY,
                                      .type = MessageSystem::Type::SCENE_MESSAGE };
        MessageSystem::sendMessage(msg);
    }
private:
    std::array<GraphicsComponent::Sprite, 3> weapons {};
    std::array<GraphicsComponent::Animation, 4> engines {};
    std::unique_ptr<std::string> score_text {};

    GraphicsComponent::Sprite ship_sprite {};    

    entt::entity ship = entt::null;
    entt::entity current_weapon = entt::null;
    entt::entity current_engine = entt::null;
    entt::entity fade_in = entt::null;
    entt::entity fade_out = entt::null;
    entt::entity score_info_label = entt::null;
    size_t current_weapon_index = 0;
    size_t current_engine_index = 0;
    Rectangle engine_rect {};
    Rectangle weapon_rect {};
    int score = 0;
};
