#include "hitbox_system.hpp"
#include "hitbox_component.hpp"

void HitboxSystem::update(entt::registry &registry)
{
    const auto &view = registry.view<HitboxComponent::Container>(); 
    for (auto [entity, container] : view.each()) {
        const auto &rect = registry.get<Rectangle>(container.parent_object);
        for (auto &hitbox : container.hitboxes) {
            hitbox.rect.x = rect.x + hitbox.x_padding; 
            hitbox.rect.y = rect.y + hitbox.y_padding; 
        }
    }
}
