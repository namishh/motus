#include "raylib.h"

int main(void)
{
    const int screenWidth = 1000;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Physics Engine");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(RAYWHITE);
            DrawText("Hello world", 190, 200, 20, BLUE);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
