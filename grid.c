#include "raylib.h"
#include "grid.h"
#include "math.h"

#define GRID_SPACING 25

void DrawCircuitGrid()
{
	int screenWidth = GetScreenWidth();
	int screenHeight = GetScreenHeight();

	for (int x = 0; x < screenWidth; x += GRID_SPACING)
	{
		DrawLine(x, 0, x, screenHeight, LIGHTGRAY);
	}

	for (int y = 0; y < screenWidth; y += GRID_SPACING)
	{
		DrawLine(0, y, screenWidth, y, LIGHTGRAY);
	}
}

Vector2 SnapToGrid(Vector2 position)
{
	Vector2 snapped;

	snapped.x = roundf(position.x / GRID_SPACING) * GRID_SPACING;
	snapped.y = roundf(position.y / GRID_SPACING) * GRID_SPACING;

	return snapped;
}

