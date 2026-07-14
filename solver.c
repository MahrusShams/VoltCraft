#include <math.h>
#include "solver.h" // Include your own header file!

#define EPSILON 1e-9 // Threshold to detect singular matrices (division by zero)

int solveMatrixDynamic(double A[MAX_MNA][MAX_MNA], double b[MAX_MNA], double x[MAX_MNA], int size) {
    
    // --- PHASE 1 & 2: FORWARD ELIMINATION WITH PIVOTING ---
    for (int i = 0; i < size; i++) {
        
        // 1. Partial Pivoting
        int maxRow = i;
        for (int k = i + 1; k < size; k++) {
            if (fabs(A[k][i]) > fabs(A[maxRow][i])) {
                maxRow = k;
            }
        }

        // Swap rows if a larger pivot is found
        if (maxRow != i) {
            for (int k = i; k < size; k++) {
                double temp = A[i][k];
                A[i][k] = A[maxRow][k];
                A[maxRow][k] = temp;
            }
            double temp = b[i];
            b[i] = b[maxRow];
            b[maxRow] = temp;
        }

        // Detect unsolvable circuit (floating nodes, shorted batteries)
        if (fabs(A[i][i]) < EPSILON) {
            return -1; 
        }

        // 2. Forward Elimination
        for (int j = i + 1; j < size; j++) {
            double factor = A[j][i] / A[i][i];
            for (int k = i; k < size; k++) {
                A[j][k] -= factor * A[i][k];
            }
            b[j] -= factor * b[i];
        }
    }

    // --- PHASE 3: BACK SUBSTITUTION ---
    for (int i = size - 1; i >= 0; i--) {
        double sum = 0.0;
        // Notice this loop also stops at 'size'
        for (int j = i + 1; j < size; j++) {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
    }

    return 0; // Success
}
