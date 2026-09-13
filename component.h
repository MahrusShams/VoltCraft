#ifndef COMPONENT_H
#define COMPONENT_H //Stop header file from being used multiple times

#include "raylib.h"
typedef struct
{
	Vector2 position;
	double value;
} Resistor;

typedef struct
{
	Vector2 position;
	double value;
} DCSource;


void DrawResistor(Vector2 position);
void DrawDCSource(Vector2 position); 

#endif