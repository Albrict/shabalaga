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
        HIGH
    };

    struct Animation {
        struct bindedFrame {
            int frame = 0;
            int tag_id = 0;
            animationCallback cb = nullptr;
        };
        Aseprite::AsepriteTag tag;
        std::vector<bindedFrame> callbacks {};
        float scale = 0.f;
        int current_tag_id = 0;
    };
    
    struct Sprite {
        Aseprite::Aseprite sprite {};
        float scale = 0.f;
        int current_frame = 0;
    };

    inline Animation createAnimation(const std::string_view &key, const unsigned int initial_tag_id, const float width, const float height)
    {
        Animation animation {};   
        animation.tag = ResourceSystem::getAsepriteTag(key, initial_tag_id);
        const float sprite_width = Aseprite::GetAsepriteWidth(animation.tag.aseprite);
        const float sprite_height = Aseprite::GetAsepriteHeight(animation.tag.aseprite);

        animation.scale = std::min(sprite_width / width, sprite_height / height);
        return animation;
    }

    inline Sprite createSprite(const std::string_view &key,  const float width, const float height)
    {
        Sprite sprite {};
        sprite.sprite = ResourceSystem::getAseprite(key); 
        const float sprite_width = Aseprite::GetAsepriteWidth(sprite.sprite);
        const float sprite_height = Aseprite::GetAsepriteHeight(sprite.sprite);
        sprite.scale = std::min(sprite_width / width, sprite_height / height);
        return sprite;
    }

    inline void addCallback(Animation &animation, const int tag_id, const int tag_frame, animationCallback cb)
    {
        animation.callbacks.push_back({tag_frame, tag_id, cb});
    }
}
