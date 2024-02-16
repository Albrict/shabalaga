#include "input_system.hpp"
#include "../include/raylib.h"
#include "input_component.hpp"

void InputSystem::proccessEvents(entt::registry &registry)
{
    auto view = registry.view<InputComponent::Container>();
    for (auto [entity, container] : view.each()) {
        auto &input_vector = container.inputs;
        for (auto &input_component : input_vector) {
            switch(input_component.type) {
            using enum InputComponent::Type; 
            case DOWN:
                if (IsKeyDown(input_component.key))
                    input_component.cb(registry, entity);
                break;
            case PRESS:
                if (IsKeyPressed(input_component.key))
                    input_component.cb(registry, entity);
            default:
                break;
            }
        }
    }
}
