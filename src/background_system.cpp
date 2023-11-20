#include "background_system.hpp"
#include "background_component.hpp"
#include "game.hpp"
#include "graphics.hpp"

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
        const Vector2 resolution = Graphics::getCurrentResolution();
        const Rectangle source_rect = { background.offset.x, background.offset.y, background.width, background.height };
        const Rectangle dest_rect = { 0.f, 0.f, resolution.x, resolution.y };
        DrawTexturePro(background.background, source_rect, dest_rect, {0.f, 0.f}, 0.f, WHITE);
    }
}
