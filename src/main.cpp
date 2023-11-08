#include "../include/raylib.h"
#include "../include/glfw3.h"

int main()
{
    InitWindow(800, 450, "monitor videomodes");

    int count;
    const GLFWvidmode* modes = (glfwGetVideoModes(glfwGetPrimaryMonitor(), &count));

    SetTargetFPS(60);

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
            
            ClearBackground(RAYWHITE);
            for (int i = 0; i < count; i++) DrawText(TextFormat("%i x %i", (int)modes[i].width, (int)modes[i].height), 10, 15*i, 10, DARKGRAY);
        EndDrawing();
    }

    CloseWindow();        // Close window and OpenGL context

    return 0;
}
