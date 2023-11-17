#include "main_menu.hpp"
#include "button_system.hpp"
#include "game.hpp"
#include "widget_components.hpp"
#include "background_component.hpp"
#include "background_system.hpp"

MainMenu::MainMenu()
{
    const std::filesystem::path bg_path = "assets/backgrounds/main_menu.png";
    const Vector2 resolution = Game::Instance()->getCurrentResolution();
    const float button_width = 150.f;
    const float button_height = 70.f;
    const float initial_x = resolution.x / 2 - button_width / 2;
    const size_t widgets = 3; 
    const std::array<const char*, 3> button_labels = { "Play", "Settings", "Exit" };
    const std::array<WidgetCallbackComponent::cb, 3> button_callback {
        {
            { entt::connect_arg<playCallback> },
            { entt::connect_arg<settingsCallback> },
            { entt::connect_arg<exitCallback> }
        }
    };
    float initial_y = resolution.y / 3 - button_height;
    for (size_t i = 0; i < widgets; ++i) {
        Rectangle button_rect = {initial_x, initial_y, button_width, button_height};
        const auto entity = m_menu_registry.create();
        m_menu_registry.emplace<ButtonComponent>(entity, ButtonComponent(button_rect, button_labels[i]));
        m_menu_registry.emplace<WidgetCallbackComponent>(entity, button_callback[i], this);
        initial_y += button_height * 2;
    }

    // Init background
    const auto background_entity = m_menu_registry.create();
    m_menu_registry.emplace<BackgroundComponent>(background_entity, 
                                                 BackgroundComponent(bg_path, 0.f, 0.f));
}

void MainMenu::proccessEvents()
{
    ButtonSystem::proccessEvents(m_menu_registry);
}

void MainMenu::update()
{
    ButtonSystem::update(m_menu_registry);
}

void MainMenu::draw() const
{
    BackgroundSystem::draw(m_menu_registry);    
    ButtonSystem::draw(m_menu_registry);
}
