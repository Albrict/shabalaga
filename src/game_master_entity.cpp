#include "game_master_entity.hpp"
#include "game_master_components.hpp"
#include "message_system.hpp"
#include "object_component.hpp"
#include "timer_component.hpp"

namespace GameMasterEntity {
    void proccessMessagesCallback(entt::registry &registry, const entt::entity entity, MessageSystem::Message msg)
    {
        auto &game_info = registry.get<GameMasterComponent::GameInfo>(entity);
        int score = entt::any_cast<int>(msg.msg);
        game_info.score += score; 
    }
    
    int defineWeapon();
    int defineEngine();
}

entt::entity GameMasterEntity::create(entt::registry &registry, const entt::entity player)
{
    using namespace GameMasterComponent;
    const auto game_master = registry.create();
    const int fighter_fleet_spawn_timer_id = 1;
    const float initial_cooldown = 2.f;
    auto &timer_container = registry.emplace<TimerComponent::Container>(game_master);
    auto &game_info = registry.emplace<GameInfo>(game_master);    

    registry.emplace<ObjectType>(game_master, ObjectType::GAME_MASTER);
    MessageSystem::registrEntity(registry, game_master, MessageSystem::Type::GAME_MASTER_MESSAGE, proccessMessagesCallback);
    TimerComponent::createTimerInContainer(timer_container, initial_cooldown, fighter_fleet_spawn_timer_id);

    game_info = {
        .current_difficulty = GameInfo::Difficulty::EASY,
        .player_entity = player,
        .score = 0,
        .scout_amount = 0,
        .engine_type = 0,
        .weapon_type = 0
    };
    return game_master;
}
