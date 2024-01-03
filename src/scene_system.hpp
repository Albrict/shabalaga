#pragma once
#include "scene.hpp"

namespace SceneSystem {
    void proccessEvents(Scene &scene);
    void update(Scene &scene);
    void draw(const Scene &scene);
}
