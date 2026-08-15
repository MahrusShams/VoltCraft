#include<raylib.h>

int main(void)
{
    const int screenWidth = 1200;
    const int screenHeight = 700;

    InitWindow(screenWidth, screenHeight, "VoltCraft");

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        BeginDrawing();

        ClearBackground(RAYWHITE);

        DrawCircuitGrid();

        EndDrawing();
    }

    CloseWindow();

    return 0;
}