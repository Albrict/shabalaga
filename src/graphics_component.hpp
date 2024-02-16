#pragma once
#include "../include/entt.hpp"
#include "resource_system.hpp"
#include "aseprite.hpp"

namespace GraphicsComponent {
    typedef void(*animationCallback)(entt::registry &registry, const entt::entity entity);

    enum class RenderType {
        SPRITE,
        ANIMATION,
        BACKGROUND,
        WIDGET,
        EFFECT
    };
    
    enum class RenderPriority {
        LOW,
        MIDDLE,
        HIGH,
        NONE
    };

    struct Animation {
        struct bindedFrame {
            int frame = 0;
            int tag_id = 0;
            animationCallback cb = nullptr;
        };
        Aseprite::AsepriteTag tag;
        std::vector<bindedFrame> callbacks {};
        int current_tag_id = 0;
    };
    
    struct Sprite {
        Aseprite::Aseprite sprite {};
        int current_frame = 0;
    };
    
    inline Sprite createSprite(const std::string_view &key)
    {
        return (Sprite) { .sprite = ResourceSystem::getAseprite(key) };
    }
    
    inline Animation createAnimation(const std::string_view &key, const int initial_tag)
    {
        return (Animation) { .tag = ResourceSystem::getAsepriteTag(key, initial_tag) };
    }

    inline void addSpriteComponent(entt::registry &registry, const entt::entity entity, const std::string_view &key, 
                                     const Rectangle rect, const RenderPriority priority)
    {
        Sprite sprite = {.sprite = ResourceSystem::getAseprite(key) };
        registry.emplace<Sprite>(entity, sprite);
        registry.emplace<GraphicsComponent::RenderPriority>(entity, priority); 
        registry.emplace<GraphicsComponent::RenderType>(entity, GraphicsComponent::RenderType::SPRITE);
    }

    inline void addAnimationComponent(entt::registry &registry, const entt::entity entity, const std::string_view &key, const unsigned int tag_id,
                                     const Rectangle rect, const RenderPriority priority)
    {
        Animation animation = { .tag = ResourceSystem::getAsepriteTag(key, tag_id) };
        registry.emplace<Animation>(entity, animation); 
        registry.emplace<GraphicsComponent::RenderPriority>(entity, priority); 
        registry.emplace<GraphicsComponent::RenderType>(entity, GraphicsComponent::RenderType::ANIMATION);
    }
    
    inline void changeAsepriteTag(Animation &animation, const unsigned int tag_id, const std::string_view &key)
    {
            animation.tag.currentFrame = 1;
            animation.tag =  ResourceSystem::getAsepriteTag(key, tag_id);
            animation.current_tag_id = tag_id; 
    }

    inline void addCallback(entt::registry &registry, const entt::entity entity, const int tag_id, const int tag_frame, animationCallback cb)
    {
        auto &animation = registry.get<Animation>(entity);
        animation.callbacks.push_back({tag_frame, tag_id, cb});
    }
}
