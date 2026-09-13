#ifndef WIRE_H
#define WIRE_H

#include "raylib.h"

typedef struct
{
	Vector2 start;
	Vector2 end;
}Wire;

void DrawWire(Vector2 start, Vector2 end);

#endif