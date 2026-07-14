#include <stdio.h>

#define MAX_POINTS 100
#define MAX_COMPONENTS 100
#define MAX_NAME 20

typedef struct
{
    char name[MAX_NAME];
    char type;      // R = resistor, V = voltage source, C = capacitor
    int pointA;
    int pointB;
    double value;
} Component;

void bfsAssignNode(int start, int totalPoints, int graph[MAX_POINTS][MAX_POINTS],
    int visited[], int nodeNumber[], int currentNode)
{
    int queue[MAX_POINTS];
    int front = 0, rear = 0;

    queue[rear++] = start;
    visited[start] = 1;

    while (front < rear)
    {
        int current = queue[front++];
        nodeNumber[current] = currentNode;

        for (int neighbor = 0; neighbor < totalPoints; neighbor++)
        {
            if (graph[current][neighbor] == 1 && visited[neighbor] == 0)
            {
                visited[neighbor] = 1;
                queue[rear++] = neighbor;
            }
        }
    }
}

void printPointMapping(int totalPoints, int nodeNumber[])
{
    printf("\nGRID POINT TO ELECTRICAL NODE MAPPING:\n");
    printf("--------------------------------------\n");

    for (int i = 0; i < totalPoints; i++)
    {
        printf("Point %d -> Node %d\n", i, nodeNumber[i]);
    }
}

void printNetlist(Component components[], int componentCount, int nodeNumber[])
{
    printf("\nGENERATED NETLIST:\n");
    printf("------------------\n");

    for (int i = 0; i < componentCount; i++)
    {
        Component c = components[i];

        printf("%s %d %d %.2lf\n",
            c.name,
            nodeNumber[c.pointA],
            nodeNumber[c.pointB],
            c.value);
    }
}

int main()
{
    int totalPoints;
    int graph[MAX_POINTS][MAX_POINTS] = { 0 };

    printf("Enter number of grid points: ");
    scanf("%d", &totalPoints);

    printf("\nGrid points will be numbered from 0 to %d\n", totalPoints - 1);

    int groundPoint;
    printf("Enter ground point number: ");
    scanf("%d", &groundPoint);

    int wireCount;
    printf("\nEnter number of wires: ");
    scanf("%d", &wireCount);

    printf("Enter each wire as: pointA pointB\n");

    for (int i = 0; i < wireCount; i++)
    {
        int a, b;
        printf("Wire %d: ", i + 1);
        scanf("%d %d", &a, &b);

        graph[a][b] = 1;
        graph[b][a] = 1;
    }

    Component components[MAX_COMPONENTS];
    int componentCount;

    printf("\nEnter number of components: ");
    scanf("%d", &componentCount);

    printf("\nEnter each component as:\n");
    printf("name type pointA pointB value\n");
    printf("Example: R1 R 0 1 100\n\n");

    for (int i = 0; i < componentCount; i++)
    {
        printf("Component %d: ", i + 1);

        scanf("%s %c %d %d %lf",
            components[i].name,
            &components[i].type,
            &components[i].pointA,
            &components[i].pointB,
            &components[i].value);
    }

    int visited[MAX_POINTS] = { 0 };
    int nodeNumber[MAX_POINTS];

    for (int i = 0; i < totalPoints; i++)
    {
        nodeNumber[i] = -1;
    }

    int currentNode = 0;

    // Ground-connected group becomes Node 0
    bfsAssignNode(groundPoint, totalPoints, graph, visited, nodeNumber, currentNode);
    currentNode++;

    // Other wire-connected groups become Node 1, Node 2, ...
    for (int i = 0; i < totalPoints; i++)
    {
        if (visited[i] == 0)
        {
            bfsAssignNode(i, totalPoints, graph, visited, nodeNumber, currentNode);
            currentNode++;
        }
    }

    printPointMapping(totalPoints, nodeNumber);
    printNetlist(components, componentCount, nodeNumber);

    return 0;
}
