#include "graphics_system.hpp"
#include "background_component.hpp"
#include "button_system.hpp"
#include "dropdown_box_system.hpp"
#include "graphics.hpp"
#include "graphics_component.hpp"
#include "label_system.hpp"
#include "collision_component.hpp"
#include "object_component.hpp"
#include "panel_system.hpp"
#include "score_label_system.hpp"
#include "slider_system.hpp"
#include "widget_components.hpp"
#include "fade_component.hpp"

namespace GraphicsSystem {
    void updateBackground(entt::registry &registry)
    {
        auto view = registry.view<BackgroundComponent::Background>();
        for (auto [entity, background] : view.each()) {
            background.offset.x += GetFrameTime() * background.scroll_speed_x;
            background.offset.y += GetFrameTime() * background.scroll_speed_y;
        }
    }
    
    void updateFade(entt::registry &registry)
    {
        auto view = registry.view<FadeEffect::Component>();
        for (auto [entity, fade] : view.each()) {
            if (!fade.paused)
                fade.lifetime -= GetFrameTime(); 
        }
    }

    void updateAnimation(entt::registry &registry)
    {
        auto view = registry.view<GraphicsComponent::Animation>();
        for (auto [entity, animation] : view.each()) {
            if (!animation.callbacks.empty()) {
                for (const auto binded_frame : animation.callbacks) {
                    if (animation.tag.tagId == binded_frame.tag_id) {
                        if (animation.tag.currentFrame == binded_frame.frame)
                            binded_frame.cb(registry, entity);
                    }
                }
            }
            Aseprite::UpdateAsepriteTag(&animation.tag);
        }
    }
     
    void renderBackground(const entt::registry &registry)
    {
        const auto &view = registry.view<BackgroundComponent::Background>();
        for (auto [entity, background] : view.each()) {
            const Vector2 resolution = Graphics::getCurrentResolution();
            const Rectangle source_rect = { background.offset.x, background.offset.y, background.width, background.height };
            const Rectangle dest_rect = { 0.f, 0.f, resolution.x, resolution.y };
            DrawTexturePro(background.background, source_rect, dest_rect, {0.f, 0.f}, 0.f, WHITE);
        }
    }
    
    void renderFade(const entt::registry &registry)
    {
        const auto &view = registry.view<FadeEffect::Component>();
        for (auto [entity, fade] : view.each()) {
            if (!fade.paused) {
                const Vector2 resolution = Graphics::getCurrentResolution();
                const Rectangle rect = { 0.f, 0.f, resolution.x, resolution.y };
                float alpha = 0.f; 
                if (fade.type == FadeEffect::Type::FADE_OUT)
                    alpha = fade.lifetime / 1.f;
                else
                    alpha = 1.f - (fade.lifetime / 1.f);
                DrawRectanglePro(rect, {0.f, 0.f}, 0.f, Fade(BLACK, alpha));
            }
        }
    }

    void renderEntity(const entt::registry &registry, const entt::entity entity)
    {
        const auto render_type = registry.get<GraphicsComponent::RenderType>(entity);
        if (render_type == GraphicsComponent::RenderType::SPRITE) {
            const auto &sprite = registry.get<GraphicsComponent::Sprite>(entity);
            const auto &dest_rect = registry.get<Rectangle>(entity);
            Aseprite::DrawAsepritePro(sprite.sprite, sprite.current_frame, dest_rect, {0.f, 0.f}, 0.f, WHITE);
        } else {
            const auto &animation = registry.get<GraphicsComponent::Animation>(entity);
            const auto &dest_rect = registry.get<Rectangle>(entity);
            Aseprite::DrawAsepriteTagPro(animation.tag, dest_rect, {0.f, 0.f}, 0.f, WHITE);
        }
    }

    void renderShipComponent(const entt::registry &registry)
    {
        const auto &view = registry.view<ObjectType>();
        for (auto [entity, type] : view.each()) {
            if (type == ObjectType::SHIP_COMPONENT)
                renderEntity(registry, entity);
        }
    }

    void renderShips(const entt::registry &registry)
    {
        const auto &view = registry.view<ObjectType>();
        for (auto [entity, type] : view.each()) {
            if (type == ObjectType::PLAYER_SHIP || type == ObjectType::ENEMY_SHIP) 
                renderEntity(registry, entity);
        }
    }
    
    void renderProjectiles(const entt::registry &registry)
    {
        const auto &view = registry.view<ObjectType>(); 
        for (auto [entity, type] : view.each()) {
            if (type == ObjectType::PROJECTILE || type == ObjectType::ENEMY_PROJECTILE)
                renderEntity(registry, entity);
        }
    }

    void renderWidget(const entt::registry &registry)
    {
        const auto &view = registry.view<WidgetComponents::Type>();
        for (auto [entity, type] : view.each()) {
            switch(type) {
            using enum WidgetComponents::Type;
            case BUTTON:
                ButtonSystem::draw(registry);
                break;
            case SLIDER:
                SliderSystem::draw(registry);
                break; 
            case DROPDOWNBOX:
                DropdownBoxSystem::draw(registry);
                break;
            case LABEL:
                LabelSystem::draw(registry);
                break;
            case PANEL:
                PanelSystem::draw(registry);
                break;
            case SCORE_LABEL:
                ScoreLabelSystem::draw(registry);
                break;
            }
        }
    }
     
    void renderHitboxes(const entt::registry &registry)
    {
        const auto &dynamic_collider_view = registry.view<Collision::DynamicComponent>();
        for (auto [entity, dynamic_collider] : dynamic_collider_view.each()) {
            for (const auto &hitbox : dynamic_collider.hitboxes)
                DrawRectangleLinesEx(hitbox.rect, 1.f, RED);
        }
        const auto &static_collider_view = registry.view<Collision::StaticComponent>();
        for (auto [entity, static_collider] : static_collider_view.each()) {
            for (const auto &hitbox : static_collider.hitboxes)
                DrawRectangleLinesEx(hitbox.rect, 1.f, RED);
        }
    }
    
    void renderEffects(const entt::registry &registry)
    {
        const auto &view = registry.view<GraphicsComponent::RenderType>(); // Bad code, we need add priority system to render
        for (auto [entity, type] : view.each()) {
            if (type == GraphicsComponent::RenderType::EFFECT) 
                renderEntity(registry, entity);
        } 
    }

    void renderPriorityLow(const entt::registry &registry) 
    {
        const auto &view = registry.view<GraphicsComponent::RenderPriority>();
        for (auto [entity, priority] : view.each()) {
            if (priority == GraphicsComponent::RenderPriority::LOW)
                renderEntity(registry, entity);
        }
    }

    void renderPriorityMiddle(const entt::registry &registry) 
    {
        const auto &view = registry.view<GraphicsComponent::RenderPriority>();
        for (auto [entity, priority] : view.each()) {
            if (priority == GraphicsComponent::RenderPriority::MIDDLE)
                renderEntity(registry, entity);
        }
    }

    void renderPriorityHigh(const entt::registry &registry) 
    {
        const auto &view = registry.view<GraphicsComponent::RenderPriority>();
        for (auto [entity, priority] : view.each()) {
            if (priority == GraphicsComponent::RenderPriority::HIGH)
                renderEntity(registry, entity);
        }
    }

    void renderUI(const entt::registry &registry)
    {
        const auto &view = registry.view<ObjectType>(); 
        for (auto [entity, type] : view.each()) {
            if (type == ObjectType::HUD) 
                renderEntity(registry, entity);
        } 
    }

}
void GraphicsSystem::update(entt::registry &registry)
{ 
    updateFade(registry);
    updateBackground(registry);
    updateAnimation(registry);
}

void GraphicsSystem::draw(const entt::registry &registry)
{
    renderBackground(registry); // render background first
    renderPriorityHigh(registry);
    renderPriorityMiddle(registry);
    renderPriorityLow(registry);
    renderWidget(registry);
    renderHitboxes(registry);
    renderUI(registry); 
    renderFade(registry);
}
