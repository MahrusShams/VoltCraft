#include "circuit.h"
#include <stdio.h>
#include <string.h>


static bool SamePoint(Vector2 a, Vector2 b)
{
    return a.x == b.x && a.y == b.y;
}

static int FindOrAddPoint(CircuitData* circuit,Vector2 position)
{
    for (int i = 0; i < circuit->pointCount; i++)  //Check if the point has same point ID
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

    //If not same, return new ID
    int newPoint = circuit->pointCount;

    circuit->points[newPoint] = position;

    circuit->pointCount++;

    return newPoint;
}

static void ConnectPoints(CircuitData* circuit,int pointA,int pointB)
{
    circuit->graph[pointA][pointB] = 1;
    circuit->graph[pointB][pointA] = 1;
}


bool BuildCircuitData(
    Wire wires[],
    int wireCount,

    Resistor resistors[],
    int resistorCount,

    DCSource sources[],
    int sourceCount,

    bool groundPlaced,
    Vector2 groundPosition,

    CircuitData* circuit)  //Convert graphics to data for netlist input
{
    memset(circuit, 0, sizeof(CircuitData)); //Clear previous circuit

    circuit->groundPoint = -1;

    // Ground is required for the netlist 
    if (!groundPlaced)
    {
        return false;
    }



    for (int i = 0; i < wireCount; i++)
    {
        int pointA = FindOrAddPoint(circuit, wires[i].start);

        int pointB = FindOrAddPoint(circuit, wires[i].end);

        if (pointA == -1 || pointB == -1)
        {
            return false;
        }

        ConnectPoints(circuit,pointA,pointB);
    }

    //Assign R input
    for (int i = 0; i < resistorCount; i++) 
    {
        if (circuit->componentCount >= MAX_COMPONENTS)
        {
            return false;
        }

        Vector2 terminalA ={resistors[i].position.x - 50,resistors[i].position.y};

        Vector2 terminalB ={resistors[i].position.x + 50,resistors[i].position.y};

        int pointA = FindOrAddPoint(circuit, terminalA);

        int pointB = FindOrAddPoint(circuit, terminalB);


        if (pointA == -1 || pointB == -1)
        {
            return false;
        }

        Component* component = &circuit->components[circuit->componentCount];

        //Assign the inputs
        snprintf(component->name,MAX_NAME,"R%d",i + 1);

        component->type = 'R';

        component->pointA = pointA;
        component->pointB = pointB;

        component->value = resistors[i].value;

        circuit->componentCount++;
    }



    //Assign DC Input
    for (int i = 0; i < sourceCount; i++)
    {
        if (circuit->componentCount >= MAX_COMPONENTS)
        {
            return false;
        }


        Vector2 positiveTerminal ={sources[i].position.x,sources[i].position.y - 50};

        Vector2 negativeTerminal ={sources[i].position.x,sources[i].position.y + 50};


        int pointA = FindOrAddPoint(circuit, positiveTerminal);

        int pointB = FindOrAddPoint(circuit, negativeTerminal);

        if (pointA == -1 || pointB == -1)
        {
            return false;
        }

        Component* component = &circuit->components[circuit->componentCount];


        snprintf(component->name,MAX_NAME,"V%d",i + 1);


        //Assign the input
        component->type = 'V';

        component->pointA = pointA;
        component->pointB = pointB;

        component->value = sources[i].value;

        circuit->componentCount++;
    }

    //Find Ground Point
    circuit->groundPoint = FindOrAddPoint(circuit, groundPosition);

    if (circuit->groundPoint == -1)
    {
        return false;
    }
    return true;
}