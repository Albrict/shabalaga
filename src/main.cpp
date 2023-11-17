#include "../include/raylib.h"
#include "../include/glfw3.h"
#include "../include/entt.hpp"
#include "game.hpp"

int main()
{
    Game::Instance()->initFirstScene();
    Game::Instance()->run();
    Game::Instance()->closeGame();
}
