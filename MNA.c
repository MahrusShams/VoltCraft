#include <stdio.h>
#include <math.h>

#include "electrical_component.h"
#include "solver.h"

int buildAndSolveCircuit(
    Component circuit[],
    int num_components,
    int num_active_nodes,
    int nodeNumber[])
{
    double A[MAX_MNA][MAX_MNA] = { 0.0 };
    double b[MAX_MNA] = { 0.0 };
    double x[MAX_MNA] = { 0.0 };

    int voltageSourceCount = 0;

    for (int i = 0; i < num_components; i++)
    {
        if (circuit[i].type == 'V' || circuit[i].type == 'v')
        {
            voltageSourceCount++;
        }
    }

    int total_size = num_active_nodes + voltageSourceCount;

    if (total_size <= 0 || total_size > MAX_MNA)
    {
        printf("Error: Circuit is too large for the solver.\n");
        return -1;
    }

    int battery_index = num_active_nodes;


   
    // Build MNA matrix
    

    for (int i = 0; i < num_components; i++)
    {
        int n1 = nodeNumber[circuit[i].pointA];
        int n2 = nodeNumber[circuit[i].pointB];

        if (circuit[i].type == 'R' || circuit[i].type == 'r')
        {
            if (circuit[i].value == 0)
            {
                printf("Error: %s has zero resistance.\n", circuit[i].name);
                return -1;
            }

            double g = 1.0 / circuit[i].value;

            if (n1 > 0)
            {
                A[n1 - 1][n1 - 1] += g;
            }

            if (n2 > 0)
            {
                A[n2 - 1][n2 - 1] += g;
            }

            if (n1 > 0 && n2 > 0)
            {
                A[n1 - 1][n2 - 1] -= g;
                A[n2 - 1][n1 - 1] -= g;
            }
        }

        else if (circuit[i].type == 'V' || circuit[i].type == 'v')
        {
            if (n1 > 0)
            {
                A[n1 - 1][battery_index] = 1.0;
                A[battery_index][n1 - 1] = 1.0;
            }

            if (n2 > 0)
            {
                A[n2 - 1][battery_index] = -1.0;
                A[battery_index][n2 - 1] = -1.0;
            }

            b[battery_index] = circuit[i].value;

            battery_index++;
        }
    }


    
    // Solve matrix
   

    int status = solveMatrixDynamic(A, b, x, total_size);

    if (status != 0)
    {
        printf("\nError: Singular Matrix (Invalid Circuit Topology).\n");
        return status;
    }


   

    printf("\n--- NODE VOLTAGES ---\n");

    printf("Voltage at Node 0: 0.000 V (Ground)\n");

    for (int i = 0; i < num_active_nodes; i++)
    {
        printf(
            "Voltage at Node %d: %.3f V\n",
            i + 1,
            x[i]
        );
    }


    
    // Calculate component currents
    

    printf("\n--- COMPONENT CURRENTS ---\n");

    double nodeCurrent[MAX_MNA] = { 0.0 };

    int sourceIndex = 0;

    for (int i = 0; i < num_components; i++)
    {
        int n1 = nodeNumber[circuit[i].pointA];
        int n2 = nodeNumber[circuit[i].pointB];

        double v1 = 0.0;
        double v2 = 0.0;

        if (n1 > 0)
        {
            v1 = x[n1 - 1];
        }

        if (n2 > 0)
        {
            v2 = x[n2 - 1];
        }


        if (circuit[i].type == 'R' || circuit[i].type == 'r')
        {
            double current =
                (v1 - v2) / circuit[i].value;

            if (current >= 0)
            {
                printf(
                    "%s: %.6f A  (Node %d -> Node %d)\n",
                    circuit[i].name,
                    current,
                    n1,
                    n2
                );
            }
            else
            {
                printf(
                    "%s: %.6f A  (Node %d -> Node %d)\n",
                    circuit[i].name,
                    -current,
                    n2,
                    n1
                );
            }

            nodeCurrent[n1] += current;
            nodeCurrent[n2] -= current;
        }


        else if (circuit[i].type == 'V' || circuit[i].type == 'v')
        {
            double current =
                x[num_active_nodes + sourceIndex];

            if (current >= 0)
            {
                printf(
                    "%s: %.6f A  (Node %d -> Node %d)\n",
                    circuit[i].name,
                    current,
                    n1,
                    n2
                );
            }
            else
            {
                printf(
                    "%s: %.6f A  (Node %d -> Node %d)\n",
                    circuit[i].name,
                    -current,
                    n2,
                    n1
                );
            }

            nodeCurrent[n1] += current;
            nodeCurrent[n2] -= current;

            sourceIndex++;
        }
    }





    return 0;
}
