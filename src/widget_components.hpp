#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

namespace WidgetComponents {
    enum class Type {
        BUTTON,
        SLIDER,
        DROPDOWNBOX,
        LABEL,
        SCORE_LABEL,
        PANEL
    };

    struct Button {
        Rectangle rect;
        const char *text;
    };
    
    struct Slider {
        Rectangle rect;
        const char *text_left;
        const char *text_right;
        float min_value;
        float max_value;
        float *value;
    };

    struct DropdownBox {
        Rectangle rect;
        const char *text;
        mutable int *active;
        bool edit_mode;
    };

    struct Panel {
        Rectangle rect;
        const char *text;
    };

    struct Label {
        Rectangle rect;
        const char *text;
    };
    
    struct ScoreLabel {
        Rectangle rect;
        const int *score_ptr;
        int old_score;
        const char *text;
    };

    struct WidgetCallback {
        typedef void(*cb)(entt::any data);
        cb callback = nullptr;
        entt::any data;
    };
    entt::entity createButton(entt::registry &object_registry, const Rectangle rect, const char *text);
    entt::entity createPanel(entt::registry &object_registry, const Rectangle rect, const char *text = nullptr);
    entt::entity createSlider(entt::registry &object_registry, const Rectangle rect, 
                              const float min_value, const float max_value, float *value, const char *text_left = nullptr, const char *text_right = nullptr);
    entt::entity createDropDownBox(entt::registry &object_registry, const Rectangle rect, const char *text, int *active);
    entt::entity createLabel(entt::registry &object_registry, const Rectangle rect, const char *text);

    entt::entity createScoreLabel(entt::registry &object_registry, const Rectangle rect, const int *score);
};

