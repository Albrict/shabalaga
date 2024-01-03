#include "game_master_entity.hpp"
#include "object_component.hpp"

entt::entity GameMasterEntity::create(entt::registry &registry, const entt::entity player)
{
    const auto game_master = registry.create();
    registry.emplace<ObjectType>(game_master, ObjectType::GAME_MASTER);
    return game_master;
}
