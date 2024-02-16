#include "message_system.hpp"
#include <map>
#include <queue>

namespace MessageSystem {
    entt::dispatcher dispatcher {};
    std::map<std::string_view, std::unique_ptr<Listener>> listeners;
    std::map<entt::entity, std::queue<Message>> entity_listeners; 
}

void MessageSystem::sendMessage(Message msg)
{
    dispatcher.enqueue(msg);  
}

void MessageSystem::sendMessageToEntity(entt::registry &registry, Message msg)
{
    auto view = registry.view<EntitySubscriptionComponent>();
    for (auto [entity, subscriptions] : view.each()) {
        if (subscriptions.type == msg.type)
            subscriptions.msg_queue.push(msg);
    }
}

void MessageSystem::registrEntity(entt::registry &registry, const entt::entity entity, Type type, EntitySubscriptionComponent::proccessMessages cb)
{
    registry.emplace<EntitySubscriptionComponent>(entity, type, cb); 
}

void MessageSystem::unregistrEntity(entt::registry &registry, const entt::entity entity, Type type)
{
    registry.remove<EntitySubscriptionComponent>(entity);
}

void MessageSystem::unregistrEntitys(entt::registry &registry)
{
    registry.clear<EntitySubscriptionComponent>();
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

void MessageSystem::updateEntites(entt::registry &registry)
{
    auto view = registry.view<EntitySubscriptionComponent>();
    for (auto [entity, subscriptions] : view.each()) {
        if (!subscriptions.msg_queue.empty()) {
            subscriptions.cb(registry, entity, subscriptions.msg_queue.front());
            subscriptions.msg_queue.pop();
        }
    }
}
