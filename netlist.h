#include "electrical_component.h"
#ifndef NETLIST_H
#define NETLIST_H

#define MAX_POINTS 100
#define MAX_COMPONENTS 100
#define MAX_NAME 20


void bfsAssignNode(
    int start,
    int totalPoints,
    int graph[MAX_POINTS][MAX_POINTS],
    int visited[],
    int nodeNumber[],
    int currentNode
);

int assignElectricalNodes(
    int totalPoints,
    int groundPoint,
    int graph[MAX_POINTS][MAX_POINTS],
    int nodeNumber[]
);

void printPointMapping(
    int totalPoints,
    int nodeNumber[]
);

void printNetlist(
    Component components[],
    int componentCount,
    int nodeNumber[]
);

#endif