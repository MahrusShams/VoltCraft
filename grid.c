#include "raylib.h"
#include "grid.h"

#define GRID_SPACING 25  //Define is handled by preprocessor, const becomes a variable

void DrawCircuitGrid()
{
    int screenWidth = GetScreenWidth();
    int screenHeight = GetScreenHeight();

    for (int x = 0; x < screenWidth; x += GRID_SPACING)
    {
        DrawLine(x, 0, x, screenHeight, LIGHTGRAY); //DrawLine(x,y to x,y) 
    }

    for (int y = 0; y < screenHeight; y += GRID_SPACING)
    {
        DrawLine(0, y, screenWidth, y, LIGHTGRAY);
    }
}