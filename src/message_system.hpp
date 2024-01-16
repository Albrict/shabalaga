#pragma once
#include <queue>
#include <string_view>
#include "../include/entt.hpp"

namespace MessageSystem {
    enum class Type {
        SCENE_MESSAGE,
        PLAY_SCENE_MESSAGE,
        PLAYER_MESSAGE,
        GAME_MASTER_MESSAGE,
        HUD_MESSAGE
    };

    enum class SceneMessage {
        MENU,
        PLAY,
        SETTINGS,
        EXIT
    };
    
    enum class PlaySceneMessage {
        PLAYER_DIED,
    };
   
    enum class PlayerMessage {
        OUT_OF_FUEL
    };
    
    enum class HudMessage {
        INCREASE_FUEL
    };

    struct Message {
        entt::any msg;
        Type type;
    };
    
    struct Listener {
        typedef void(*proccessMessagesCallback)(Message msg);
        void proccessMessages(Message msg)
        { cb(msg); }
        proccessMessagesCallback cb;
    };
    
    struct EntitySubscriptionComponent {
        typedef void(*proccessMessages)(entt::registry &registry, const entt::entity, Message msg);
        Type type;
        proccessMessages cb = nullptr;
        std::queue<Message> msg_queue {};
    };

    void registrEntity(entt::registry &registry, const entt::entity, Type type, EntitySubscriptionComponent::proccessMessages cb);
    void unregistrEntity(entt::registry &registry, const entt::entity, Type type);
    void unregistrEntitys(entt::registry &registry);

    void registrListener(std::unique_ptr<Listener> listener, const std::string_view &key);
    void unregistrListener(const std::string_view &key);
    void sendMessage(Message msg);
    void sendMessageToEntity(entt::registry &registry, Message msg);
    void updateEntites(entt::registry &registry);
    void update();
}
