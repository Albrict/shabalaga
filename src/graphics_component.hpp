#pragma once
#include "../include/entt.hpp"
#include "resource_system.hpp"
#include "sprite.hpp"

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
        std::unique_ptr<Sprite> sprite {};
        std::vector<bindedFrame> callbacks {};
        float scale = 0.f;
    };
    
    struct Sprite {
        std::unique_ptr<::Sprite> sprite {};
        float scale = 0.f;
    };

    inline Animation createAnimation(const std::string_view &key, const char *inital_tag, const float width, const float height)
    {
        Animation animation {};   
        animation.sprite = std::make_unique<::Sprite>();
        animation.sprite->loadAseprite(ResourceSystem::getSprite(key));
        animation.sprite->loadAsepriteTag(inital_tag);

        const float sprite_width = animation.sprite->getWidth();
        const float sprite_height = animation.sprite->getWidth();

        animation.scale = std::min(sprite_width / width, sprite_height / height);
        return animation;
    }

    inline Sprite createSprite(const std::string_view &key,  const float width, const float height)
    {
        Sprite sprite {};
        sprite.sprite = std::make_unique<::Sprite>(ResourceSystem::getSprite(key));
        const float sprite_width = sprite.sprite->getWidth();
        const float sprite_height = sprite.sprite->getWidth();

        sprite.scale = std::min(sprite_width / width, sprite_height / height);
        return sprite;
    }

    inline void addCallback(Animation &animation, const int tag_id, const int tag_frame, animationCallback cb)
    {
        animation.callbacks.push_back({tag_frame, tag_id, cb});
    }
}
