#include "message_system.hpp"
#include <map>

namespace MessageSystem {
    entt::dispatcher dispatcher {};
    std::map<std::string_view, std::unique_ptr<Listener>> listeners;
}

void MessageSystem::sendMessage(Message msg)
{
    dispatcher.enqueue(msg);  
}

void MessageSystem::registrListener(std::unique_ptr<Listener> listener, const std::string_view &key)
{
    dispatcher.sink<Message>().connect<&Listener::proccessMessages>(*listener);
    listeners.insert({key, std::move(listener)});
}

void MessageSystem::unregistrListener(const std::string_view &key)
{
    listeners.erase(key);
}

void MessageSystem::update()
{
    dispatcher.update();
}
