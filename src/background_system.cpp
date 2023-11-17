#include "background_system.hpp"
#include "background_component.hpp"
#include "game.hpp"

void BackgroundSystem::clear(entt::registry &registry)
{
    auto view = registry.view<BackgroundComponent>();
    for (auto [entity, background] : view.each()) {
        UnloadTexture(background.background);
        registry.destroy(entity);
    }
}

void BackgroundSystem::update(entt::registry &registry)
{
    auto view = registry.view<BackgroundComponent>();
    for (auto [entity, background] : view.each()) {
        background.offset.x += GetFrameTime() * background.scroll_speed_x;
        background.offset.y += GetFrameTime() * background.scroll_speed_y;
    }
}

void BackgroundSystem::draw(const entt::registry &registry)
{
    auto view = registry.view<BackgroundComponent>();
    for (auto [entity, background] : view.each()) {
        // Draw background twice
        const Vector2 resolution = Game::Instance()->getCurrentResolution();
        const Rectangle source_rect = { background.offset.x, background.offset.y, background.width, background.height };
        const Rectangle dest_rect = { 0.f, 0.f, resolution.x, resolution.y };
        DrawTexturePro(background.background, source_rect, dest_rect, {0.f, 0.f}, 0.f, WHITE);
    }
}
