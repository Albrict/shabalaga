#include "weapon_system.hpp"
#include "weapon_component.hpp"

namespace {
    void update_auto_cannon_frames(struct WeaponComponent &weapon)
    {
        switch(weapon.state) {
        using enum WeaponState;
        case FIRING:
            if (weapon.sprite.getCurrentTagFrame() == 5) {
                weapon.sprite.loadAsepriteTag("Idle");
                weapon.state = IDLE;
            }
            break;
        default:
            break;
        }
    }
};

void WeaponSystem::proccessEvents(entt::registry &registry)
{
    ;
}

void WeaponSystem::update(entt::registry &registry)
{
    auto view = registry.view<WeaponComponent>(); 
    for (auto [entity, weapon] : view.each()) {
        switch(weapon.type) {
        using enum WeaponType;
        case AUTO_CANNON:
            update_auto_cannon_frames(weapon);
            break;
        case BIG_SPACE_GUN:
            break;
        case ZAPPER:
            break;
        }
        weapon.sprite.updateTag();
    }
}

void WeaponSystem::draw(const entt::registry &registry, const Rectangle dest)
{
    auto view = registry.view<WeaponComponent>(); 
    for (auto [entity, weapon] : view.each())
        weapon.sprite.drawTagPro(dest, {0.f, 0.f}, 0.f, WHITE);
}
