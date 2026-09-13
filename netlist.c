#include <stdio.h>
#include "netlist.h"


// ---------------------------------------------------------
// BFS
// Finds all grid points connected together by wires
// and assigns them to the same electrical node.
// ---------------------------------------------------------

void bfsAssignNode(
    int start,
    int totalPoints,
    int graph[MAX_POINTS][MAX_POINTS],
    int visited[],
    int nodeNumber[],
    int currentNode)
{
    int queue[MAX_POINTS];

    int front = 0;
    int rear = 0;

    queue[rear++] = start;
    visited[start] = 1;

    while (front < rear)
    {
        int current = queue[front++];

        nodeNumber[current] = currentNode;

        for (int neighbor = 0; neighbor < totalPoints; neighbor++)
        {
            if (graph[current][neighbor] == 1 &&
                visited[neighbor] == 0)
            {
                visited[neighbor] = 1;
                queue[rear++] = neighbor;
            }
        }
    }
}


// ---------------------------------------------------------
// Assign electrical nodes
//
// Ground-connected points become Node 0.
//
// Every other group of wire-connected points becomes
// Node 1, Node 2, Node 3, ...
//
// Returns the total number of electrical nodes.
// ---------------------------------------------------------

int assignElectricalNodes(
    int totalPoints,
    int groundPoint,
    int graph[MAX_POINTS][MAX_POINTS],
    int nodeNumber[])
{
    int visited[MAX_POINTS] = { 0 };

    // Initially no point belongs to an electrical node
    for (int i = 0; i < totalPoints; i++)
    {
        nodeNumber[i] = -1;
    }

    int currentNode = 0;


    // -----------------------------------------------------
    // Ground-connected group becomes Node 0
    // -----------------------------------------------------

    bfsAssignNode(
        groundPoint,
        totalPoints,
        graph,
        visited,
        nodeNumber,
        currentNode
    );

    currentNode++;


    // -----------------------------------------------------
    // Find remaining wire-connected groups
    // -----------------------------------------------------

    for (int i = 0; i < totalPoints; i++)
    {
        if (visited[i] == 0)
        {
            bfsAssignNode(
                i,
                totalPoints,
                graph,
                visited,
                nodeNumber,
                currentNode
            );

            currentNode++;
        }
    }


    return currentNode;
}


// ---------------------------------------------------------
// Debugging function
// Prints which electrical node each grid point belongs to.
// ---------------------------------------------------------

void printPointMapping(
    int totalPoints,
    int nodeNumber[])
{
    printf("\nGRID POINT TO ELECTRICAL NODE MAPPING:\n");
    printf("--------------------------------------\n");

    for (int i = 0; i < totalPoints; i++)
    {
        printf(
            "Point %d -> Node %d\n",
            i,
            nodeNumber[i]
        );
    }
}


// ---------------------------------------------------------
// Prints generated circuit netlist
// ---------------------------------------------------------

void printNetlist(
    Component components[],
    int componentCount,
    int nodeNumber[])
{
    printf("\nGENERATED NETLIST:\n");
    printf("------------------\n");

    for (int i = 0; i < componentCount; i++)
    {
        Component c = components[i];

        printf(
            "%s %d %d %.2lf\n",
            c.name,
            nodeNumber[c.pointA],
            nodeNumber[c.pointB],
            c.value
        );
    }
}