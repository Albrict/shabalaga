#include "game_master_system.hpp"
#include "graphics.hpp"
#include "timer_component.hpp"
#include "timer_system.hpp"
#include "fleet.hpp"

namespace GameMasterSystem {
    unsigned int difficulty = 1;
    int score = 0;
    
    // Internal timer that belongs only to this module
    TimerComponent::Component fleet_spawn_duration {
        .id = 1,
        .lifetime = 2.f,
        .saved_lifetime = 2.f
    };
}    

void GameMasterSystem::proccessEvents(entt::registry &registry, const entt::entity game_master)
{
    if (TimerComponent::isDone(fleet_spawn_duration)) {
        const Vector2 resolution = Graphics::getCurrentResolution();
        const int max_rand = static_cast<int>(resolution.x / 1.2f);
        const int rand_ship_amount = GetRandomValue(1, 4);
        const float ship_size = resolution.x / 8.f;
        float rand_x = static_cast<float>(GetRandomValue(ship_size, max_rand)); 
        
        if (rand_x + ship_size * rand_ship_amount >= resolution.x)
            rand_x -= (ship_size * rand_ship_amount);
        Fleet::createFleet(registry, {rand_x, -200.f}, rand_ship_amount, Fleet::Type::FIGHTER_FLEET_LONG_LINE);
        TimerComponent::reset(fleet_spawn_duration);
    }
}

void GameMasterSystem::update(entt::registry &registry, const entt::entity game_master)
{
    TimerSystem::update(fleet_spawn_duration);
}

void GameMasterSystem::increaseScore(const int amount)
{
    score += amount;
}

void GameMasterSystem::resetSystem()
{
    difficulty = 1;
    score = 0;
}

const int *GameMasterSystem::getScorePointer()
{
    return &score;
}
