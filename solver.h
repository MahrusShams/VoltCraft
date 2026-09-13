#ifndef SOLVER_H
#define SOLVER_H

#include "electrical_component.h"

#define MAX_MNA 200

int solveMatrixDynamic(
    double A[MAX_MNA][MAX_MNA],
    double b[MAX_MNA],
    double x[MAX_MNA],
    int size
);

int buildAndSolveCircuit(
    Component circuit[],
    int num_components,
    int num_active_nodes,
    int nodeNumber[]
);

#endif