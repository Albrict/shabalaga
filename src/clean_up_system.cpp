#include "clean_up_system.hpp"
#include "clean_up_component.hpp"

void CleanUpSystem::clean(entt::registry &registry)
{
    auto view = registry.view<CleanUpComponent::Component>();
    for (auto [entity] : view.each())
        registry.destroy(entity);
}
