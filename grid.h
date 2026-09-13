#ifndef GRID_H
#define GRID_H //Stop header file from being used multiple times

#include "raylib.h"

void DrawCircuitGrid();

Vector2 SnapToGrid(Vector2 position);

#endif