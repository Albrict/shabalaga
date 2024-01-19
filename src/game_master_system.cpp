#include "game_master_system.hpp"
#include "bomber_entity.hpp"
#include "game_master_components.hpp"
#include "graphics.hpp"
#include "scout_entity.hpp"
#include "timer_component.hpp"
#include "fleet.hpp"

namespace GameMasterSystem {
    enum SpawnDirection : int {
        LEFT,
        RIGHT,
        UP
    };

    void proccessFighterSpawn(entt::registry &registry, const entt::entity game_master, TimerComponent::Container &container)
    {
        const int timer_id = 1;
        if (TimerComponent::isDone(container, timer_id)) {
            const auto &game_info = registry.get<GameMasterComponent::GameInfo>(game_master);
            const Vector2 resolution = Graphics::getCurrentResolution();
            const int rand_ship_amount = GetRandomValue(1, 4);
            const float ship_size = resolution.x / 8.f;
            float rand_x = GetRandomValue(ship_size, resolution.x);
                
            if (rand_x + ship_size * rand_ship_amount >= resolution.x)
                rand_x -= (ship_size * rand_ship_amount);
            Fleet::createFleet(registry, {rand_x, -200.f}, rand_ship_amount, Fleet::Type::FIGHTER_FLEET, game_info.current_difficulty);
            TimerComponent::reset(container, timer_id);
        }
    }
    
    void proccessBomberSpawn(entt::registry &registry, const entt::entity game_master, const entt::entity player, TimerComponent::Container &container)
    {
        const int timer_id = 2;
        if (!registry.valid(player))
            return;
        else {
            if (TimerComponent::isDone(container, timer_id)) {
                const SpawnDirection direction = static_cast<SpawnDirection>(GetRandomValue(LEFT, UP));
                const Vector2 resolution = Graphics::getCurrentResolution();
                const Rectangle player_position = registry.get<Rectangle>(player);
                const float ship_size = resolution.x / 12.f;
                float rand_x = GetRandomValue(ship_size, resolution.x);
                if (rand_x + ship_size >= resolution.x)
                    rand_x -= (ship_size);
                BomberEntity::create(registry, {rand_x, -200.f, ship_size, ship_size}, player_position);
                TimerComponent::reset(container, timer_id);
            }
        }
    }

    void proccessScoutSpawn(entt::registry &registry, const entt::entity game_master)
    {
        const int scout_timer = 3;
        auto &container = registry.get<TimerComponent::Container>(game_master);
        if (TimerComponent::isDone(container, scout_timer)) {
            const Vector2 resolution = Graphics::getCurrentResolution();
            const float ship_size = resolution.x / 8.f;
            float first_ship_rand_x = GetRandomValue(ship_size, resolution.x / 2.f);
            float second_ship_rand_x = resolution.x + GetRandomValue(ship_size, resolution.x / 2.f);
            if (second_ship_rand_x + ship_size >= resolution.x)
                second_ship_rand_x -= (ship_size);
            ScoutEntity::create(registry, {first_ship_rand_x, -200.f, ship_size, ship_size});
            ScoutEntity::create(registry, {second_ship_rand_x, -200.f, ship_size, ship_size});
            TimerComponent::reset(container, scout_timer);
        }
    }

    void changeDifficultyToPreMedium(GameMasterComponent::GameInfo &game_info, TimerComponent::Container &container)
    {
        const int bomber_spawn_timer_id = 2;
        const float bomber_spawn_cooldown = 5.f;
        game_info.current_difficulty = GameMasterComponent::GameInfo::Difficulty::PRE_MEDIUM; 
        TimerComponent::createTimerInContainer(container, bomber_spawn_cooldown, bomber_spawn_timer_id);
    }

    void changeDifficultyToMedium(GameMasterComponent::GameInfo &game_info, TimerComponent::Container &container)
    {
        const int timer_id = 3;
        const float scout_fleet_spawn_cooldown = 10.f;
        game_info.current_difficulty = GameMasterComponent::GameInfo::Difficulty::MEDIUM; 
        TimerComponent::createTimerInContainer(container, scout_fleet_spawn_cooldown, timer_id);
    }
}    

void GameMasterSystem::proccessEvents(entt::registry &registry, const entt::entity game_master)
{
    const auto &game_info = registry.get<GameMasterComponent::GameInfo>(game_master);
    auto &container = registry.get<TimerComponent::Container>(game_master);

    switch(game_info.current_difficulty) {
    using  enum GameMasterComponent::GameInfo::Difficulty;
    case BOSS:
        break;
    case HARD:
        [[fallthrough]];
    case PRE_HARD:
        [[fallthrough]];
    case MEDIUM:
        proccessScoutSpawn(registry, game_master);
        [[fallthrough]];
    case PRE_MEDIUM:
        proccessBomberSpawn(registry, game_master, game_info.player_entity, container);
        [[fallthrough]];
    case EASY:
        proccessFighterSpawn(registry, game_master, container);
        break;
    }
}

void GameMasterSystem::update(entt::registry &registry, const entt::entity game_master)
{
    auto &game_info = registry.get<GameMasterComponent::GameInfo>(game_master);

    switch(game_info.current_difficulty) {
    using  enum GameMasterComponent::GameInfo::Difficulty;
    case EASY:
        if (game_info.score >= 100) 
            changeDifficultyToPreMedium(game_info, registry.get<TimerComponent::Container>(game_master));
        break;
    case PRE_MEDIUM:
        if (game_info.score >= 200)
            changeDifficultyToMedium(game_info, registry.get<TimerComponent::Container>(game_master));
        break;
    case MEDIUM:
        if (game_info.score >= 12000)
            game_info.current_difficulty = GameMasterComponent::GameInfo::Difficulty::PRE_HARD;
        break;
    case PRE_HARD:
        if (game_info.score >= 22000)
            game_info.current_difficulty = GameMasterComponent::GameInfo::Difficulty::HARD;
        break;
    case HARD:
        if (game_info.score >= 32000)
            game_info.current_difficulty = GameMasterComponent::GameInfo::Difficulty::BOSS;
        break;
    case BOSS:
        ; // Boss fight 
        break;
    }
}
