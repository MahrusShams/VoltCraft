#include "wire.h"

void DrawWire(Vector2 start, Vector2 end)
{
	DrawLineEx(start, end, 3, BLACK);

	DrawCircleV(start, 4, BLACK);
	DrawCircleV(end, 4, BLACK);

	//DrawLineV = normal line , DrawLineEx adds thickness
}