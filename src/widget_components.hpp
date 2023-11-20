#pragma once
#include "../include/entt.hpp"
#include "../include/raylib.h"

enum class WidgetComponentType {
    BUTTON,
    SLIDER,
    DROPDOWNBOX,
    LABEL,
    PANEL
};

struct ButtonComponent {
    const Rectangle rect;
    const char *text;
    ButtonComponent(const Rectangle button_rect, const char *button_text = nullptr)
        : rect(button_rect), 
        text(button_text)  {}
};

struct SliderComponent {
    const Rectangle rect;
    const char *text_left;
    const char *text_right;
    const float min_value;
    const float max_value;
    float *value;
    
    SliderComponent(const Rectangle slider_rect, const float min, const float max,
                    float *value, const char *text_left = nullptr, const char *text_right = nullptr)
    :   rect(slider_rect),
        text_left(text_left),
        text_right(text_right),
        min_value(min),
        max_value(max),
        value(value) {}
};

struct DropdownBoxComponent {
    const Rectangle rect;
    const char *text;
    mutable int *active;
    bool edit_mode;

    DropdownBoxComponent(const Rectangle dropdownbox_rect, const char *text, int *choosen)
        : rect(dropdownbox_rect), 
        text(text),
        active(choosen),
        edit_mode(false) {}
};

struct PanelComponent {
    const Rectangle rect;
    const char *text;

    PanelComponent(const Rectangle rect, const char *text = nullptr)
        : rect(rect), 
        text(text) {}
};

struct LabelComponent {
    const Rectangle rect;
    const char *text;

    LabelComponent(const Rectangle label_rect, const char *text)
        : rect(label_rect),
            text(text) {}
};

struct WidgetCallbackComponent {
    using cb = entt::delegate<void(entt::any data)>;
    cb callback;
    entt::any data;
};
