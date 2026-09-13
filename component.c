#include "component.h"
#include "raylib.h"

void DrawResistor(Vector2 position)
{
	DrawLineEx((Vector2){position.x - 50, position.y},
			   (Vector2){position.x - 30, position.y},
				3.0f, BLACK);

	Vector2 p1 = { position.x - 30, position.y };
	Vector2 p2 = { position.x - 20, position.y - 10 };
	Vector2 p3 = { position.x - 10, position.y + 10 };
	Vector2 p4 = { position.x,      position.y - 10 };
	Vector2 p5 = { position.x + 10, position.y + 10 };
	Vector2 p6 = { position.x + 20, position.y - 10 };
	Vector2 p7 = { position.x + 30, position.y };

	DrawLineEx(p1, p2, 3.0f, BLACK);
	DrawLineEx(p2, p3, 3.0f, BLACK);
	DrawLineEx(p3, p4, 3.0f, BLACK);
	DrawLineEx(p4, p5, 3.0f, BLACK);
	DrawLineEx(p5, p6, 3.0f, BLACK);
	DrawLineEx(p6, p7, 3.0f, BLACK);

	DrawLineEx((Vector2){position.x + 30, position.y},
			   (Vector2){position.x + 50, position.y},
				3.0f, BLACK);

	DrawCircleV((Vector2) { position.x - 50, position.y }, 4, BLACK);
	DrawCircleV((Vector2) { position.x + 50, position.y }, 4, BLACK);
}

void DrawDCSource(Vector2 position)
{
	DrawCircleLines(position.x, position.y, 25, BLACK);

	DrawLineEx((Vector2) {position.x, position.y - 50},
			   (Vector2) {position.x, position.y - 25},
				3.0f,BLACK);

	DrawLineEx((Vector2) {position.x, position.y + 25},
			   (Vector2) {position.x, position.y + 50},
				3.0f,BLACK);

	DrawText("+", position.x - 5, position.y - 18, 20, BLACK);
	DrawText("-", position.x - 4, position.y + 2, 20, BLACK);


	DrawCircleV((Vector2) { position.x, position.y - 50 }, 4, BLACK);
	DrawCircleV((Vector2) { position.x, position.y + 50 }, 4, BLACK);
}