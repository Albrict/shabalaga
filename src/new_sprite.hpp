#pragma once
#include "aseprite.hpp"
#include "../include/entt.hpp"

namespace GraphicsComponent {
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

    struct Sprite {
        Aseprite::Aseprite sprite {};
        unsigned int current_frame = 0;
    };
    
    struct Animation {
        typedef void(*animationCallback)(entt::registry &registry, const entt::entity entity);
        struct bindedFrame {
            int frame = 0;
            int tag_id = 0;
            animationCallback cb = nullptr;
        };
        Aseprite::AsepriteTag tag {};
        std::vector<bindedFrame> callbacks {}; 
    };

    inline Sprite createFromAseprite(const Aseprite::Aseprite aseprite)
    {
        return (Sprite) {
            .sprite = aseprite,
            .current_frame = 0,
        };
    }
    
    inline Animation createFromAseprteTag(const Aseprite::AsepriteTag tag)
    {
        return (Animation) { .tag = tag };
    }
}
