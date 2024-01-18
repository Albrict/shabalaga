#include "fighter_entity.hpp"
#include "graphics.hpp"
#include "fleet.hpp"

namespace Fleet {
    void createFighterFleet(entt::registry &registry, const Vector2 position, const unsigned int ship_amount,
                            const GameMasterComponent::GameInfo::Difficulty difficulty)
    {
        using enum GameMasterComponent::GameInfo::Difficulty;
        const Vector2 resolution = Graphics::getCurrentResolution(); 
        const float ship_size = resolution.x / 8.f;
        float fire_cooldown = 0.f;        
        Rectangle rect = {position.x, position.y, ship_size, ship_size };
        if (difficulty == MEDIUM)
            fire_cooldown = 0.8f;
        else
            fire_cooldown = 1.f;
        for (size_t i = 0; i < ship_amount; ++i) {
            const auto fighter_entity = FighterEntity::create(registry, rect, fire_cooldown);
            rect.x += ship_size; 
        }
    }
}

void Fleet::createFleet(entt::registry &registry, const Vector2 position, const size_t ship_amount, 
                const Type type, const GameMasterComponent::GameInfo::Difficulty difficulty)
{
    switch(type) {
    using enum Fleet::Type;
    case Type::FIGHTER_FLEET:
        createFighterFleet(registry, position, ship_amount, difficulty);
        break;
    }
}
