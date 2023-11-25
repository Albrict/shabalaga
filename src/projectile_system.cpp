#include "projectile_system.hpp"
#include "projectile_component.hpp"


void ProjectileSystem::update(entt::registry &registry)
{
    auto view = registry.view<ProjectileComponent>();
    for (auto [entity, projectile] : view.each()) {
        projectile.position.y -= 180.f * GetFrameTime();
        projectile.sprite.updateTag();
    }

}

void ProjectileSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<ProjectileComponent>();
    for (auto [entity, projectile] : view.each()) {
        const Rectangle rect = {projectile.position.x, projectile.position.y, 70.f, 70.f};
        projectile.sprite.drawTagPro(rect, {0.f, 0.f}, 0.f, WHITE);
    }
}
