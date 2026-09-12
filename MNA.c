#include <stdio.h>
#include "components.h"
#include "solver.h" 

int buildAndSolveCircuit(Component circuit[], int num_components, int num_active_nodes) {
    double A[MAX_MNA][MAX_MNA] = {0.0};
    double b[MAX_MNA] = {0.0};
    double x[MAX_MNA] = {0.0};
    
    int battery_index = num_active_nodes; 

    for (int i = 0; i < num_components; i++) {
        // Read the logical nodes mapped by your BFS
        int n1 = circuit[i].pointA; 
        int n2 = circuit[i].pointB;

        if (circuit[i].type == 'R' || circuit[i].type == 'r') {
            double g = 1.0 / circuit[i].value;
            if (n1 > 0) A[n1-1][n1-1] += g; 
            if (n2 > 0) A[n2-1][n2-1] += g;
            if (n1 > 0 && n2 > 0) {
                A[n1-1][n2-1] -= g;
                A[n2-1][n1-1] -= g;
            }
        } 
        else if (circuit[i].type == 'V' || circuit[i].type == 'v') {
            if (n1 > 0) {
                A[n1-1][battery_index] = 1.0;
                A[battery_index][n1-1] = 1.0;
            }
            if (n2 > 0) {
                A[n2-1][battery_index] = -1.0;
                A[battery_index][n2-1] = -1.0;
            }
            b[battery_index] = circuit[i].value;
            battery_index++; 
        }
    }

    int total_size = battery_index;
    int status = solveMatrixDynamic(A, b, x, total_size);

    if (status == 0) {
        printf("\n--- SIMULATION RESULTS ---\n");
        for (int i = 0; i < num_active_nodes; i++) {
            printf("Voltage at Node %d: %.3f V\n", i + 1, x[i]);
        }
    } else {
        printf("Error: Singular Matrix (Invalid Circuit Topology).\n");
    }

    return status;
}