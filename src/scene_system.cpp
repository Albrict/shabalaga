#include "scene_system.hpp"

void SceneSystem::proccessEvents(Scene &scene)
{
    switch(scene.type) {
    using enum SceneType;
    case PLAY_SCENE:
        break;
    case SETTINGS_SCENE:
        break;
    }
}

void SceneSystem::update(Scene &scene)
{
    switch(scene.type) {
    using enum SceneType;
    case PLAY_SCENE:
        break;
    case SETTINGS_SCENE:
        break;
    }
}

void SceneSystem::draw(const Scene &scene)
{
    switch(scene.type) {
    using enum SceneType;
    case PLAY_SCENE:
        break;
    case SETTINGS_SCENE:
        break;
    }
}
