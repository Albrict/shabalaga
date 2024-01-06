#include "score_label_system.hpp"
#include "widget_components.hpp"
#include "../include/raygui.h"

using WidgetComponents::ScoreLabel;

void ScoreLabelSystem::update(entt::registry &registry)
{
    auto view = registry.view<ScoreLabel>();
    for (const auto [entity, label] : view.each()) {
        if (label.old_score != *label.score_ptr) {
            label.old_score = *label.score_ptr;
            label.text = TextFormat("%d", label.old_score);
        }
    }
}

void ScoreLabelSystem::draw(const entt::registry &registry)
{
    
    auto view = registry.view<const ScoreLabel>();
    for (const auto [entity, label] : view.each()) 
        GuiLabel(label.rect, label.text);
}
