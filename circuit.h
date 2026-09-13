#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "raylib.h"
#include "wire.h"
#include "component.h"
#include "netlist.h"

typedef struct
{
    Vector2 points[MAX_POINTS];
    int pointCount;

    int graph[MAX_POINTS][MAX_POINTS];

    Component components[MAX_COMPONENTS];
    int componentCount;

    int groundPoint;

} CircuitData;


bool BuildCircuitData(
    Wire wires[],
    int wireCount,

    Resistor resistors[],
    int resistorCount,

    DCSource sources[],
    int sourceCount,

    bool groundPlaced,
    Vector2 groundPosition,

    CircuitData* circuit
);

#endif