#include "circuit.h"

#include <stdio.h>
#include <string.h>


// ---------------------------------------------------------
// Check whether two snapped screen positions are identical
// ---------------------------------------------------------

static bool SamePoint(Vector2 a, Vector2 b)
{
    return a.x == b.x && a.y == b.y;
}


// ---------------------------------------------------------
// Find an existing electrical point.
//
// If it does not exist, create a new point.
//
// Returns the point ID.
// Returns -1 if MAX_POINTS is exceeded.
// ---------------------------------------------------------

static int FindOrAddPoint(
    CircuitData* circuit,
    Vector2 position)
{
    for (int i = 0; i < circuit->pointCount; i++)
    {
        if (SamePoint(circuit->points[i], position))
        {
            return i;
        }
    }


    if (circuit->pointCount >= MAX_POINTS)
    {
        return -1;
    }


    int newPoint = circuit->pointCount;

    circuit->points[newPoint] = position;

    circuit->pointCount++;

    return newPoint;
}


// ---------------------------------------------------------
// Connect two electrical points using a wire
// ---------------------------------------------------------

static void ConnectPoints(
    CircuitData* circuit,
    int pointA,
    int pointB)
{
    if (pointA < 0 || pointB < 0)
    {
        return;
    }

    circuit->graph[pointA][pointB] = 1;
    circuit->graph[pointB][pointA] = 1;
}


// ---------------------------------------------------------
// Convert the GUI circuit into electrical circuit data
// ---------------------------------------------------------

bool BuildCircuitData(
    Wire wires[],
    int wireCount,

    Resistor resistors[],
    int resistorCount,

    DCSource sources[],
    int sourceCount,

    bool groundPlaced,
    Vector2 groundPosition,

    CircuitData* circuit)
{
    memset(circuit, 0, sizeof(CircuitData));

    circuit->groundPoint = -1;


    // Ground is required for the netlist BFS
    if (!groundPlaced)
    {
        return false;
    }


    // -----------------------------------------------------
    // Convert wires into graph connections
    // -----------------------------------------------------

    for (int i = 0; i < wireCount; i++)
    {
        int pointA =
            FindOrAddPoint(circuit, wires[i].start);

        int pointB =
            FindOrAddPoint(circuit, wires[i].end);


        if (pointA == -1 || pointB == -1)
        {
            return false;
        }


        ConnectPoints(
            circuit,
            pointA,
            pointB
        );
    }


    // -----------------------------------------------------
    // Convert resistors
    //
    // Current resistor is horizontal:
    //
    // terminal A = center - 50 pixels
    // terminal B = center + 50 pixels
    // -----------------------------------------------------

    for (int i = 0; i < resistorCount; i++)
    {
        if (circuit->componentCount >= MAX_COMPONENTS)
        {
            return false;
        }


        Vector2 terminalA =
        {
            resistors[i].position.x - 50,
            resistors[i].position.y
        };

        Vector2 terminalB =
        {
            resistors[i].position.x + 50,
            resistors[i].position.y
        };


        int pointA =
            FindOrAddPoint(circuit, terminalA);

        int pointB =
            FindOrAddPoint(circuit, terminalB);


        if (pointA == -1 || pointB == -1)
        {
            return false;
        }


        Component* component =
            &circuit->components[circuit->componentCount];


        snprintf(
            component->name,
            MAX_NAME,
            "R%d",
            i + 1
        );

        component->type = 'R';

        component->pointA = pointA;
        component->pointB = pointB;

        component->value = resistors[i].value;


        circuit->componentCount++;
    }


    // -----------------------------------------------------
    // Convert DC voltage sources
    //
    // Top terminal = positive
    // Bottom terminal = negative
    // -----------------------------------------------------

    for (int i = 0; i < sourceCount; i++)
    {
        if (circuit->componentCount >= MAX_COMPONENTS)
        {
            return false;
        }


        Vector2 positiveTerminal =
        {
            sources[i].position.x,
            sources[i].position.y - 50
        };

        Vector2 negativeTerminal =
        {
            sources[i].position.x,
            sources[i].position.y + 50
        };


        int pointA =
            FindOrAddPoint(circuit, positiveTerminal);

        int pointB =
            FindOrAddPoint(circuit, negativeTerminal);


        if (pointA == -1 || pointB == -1)
        {
            return false;
        }


        Component* component =
            &circuit->components[circuit->componentCount];


        snprintf(
            component->name,
            MAX_NAME,
            "V%d",
            i + 1
        );

        component->type = 'V';

        component->pointA = pointA;
        component->pointB = pointB;

        component->value = sources[i].value;


        circuit->componentCount++;
    }


    // -----------------------------------------------------
    // Ground
    // -----------------------------------------------------

    circuit->groundPoint =
        FindOrAddPoint(circuit, groundPosition);


    if (circuit->groundPoint == -1)
    {
        return false;
    }


    return true;
}