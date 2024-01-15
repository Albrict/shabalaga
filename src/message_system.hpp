#pragma once
#include <string_view>
#include "../include/entt.hpp"

namespace MessageSystem {
    enum class Type {
        SCENE_MESSAGE,
        PLAY_SCENE_MESSAGE
    };
    enum class SceneMessage {
        MENU,
        PLAY,
        SETTINGS,
        EXIT
    };
    
    enum class PlaySceneMessage {
        PLAYER_DIED,
        OUT_OF_FUEL
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
    
    void registrListener(std::unique_ptr<Listener> listener, const std::string_view &key);
    void unregistrListener(const std::string_view &key);
    void sendMessage(Message msg);
    void update();
}
