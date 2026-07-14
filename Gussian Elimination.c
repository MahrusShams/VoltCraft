#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 3          
#define EPSILON 1e-9 // Threshold to detect singular matrices (division by zero)

// Function to print a matrix and vector side-by-side (Great for debugging!)
void printSystem(double A[N][N], double b[N])
{
    for (int i = 0; i < N; i++)
    {
        printf("[ ");
        for (int j = 0; j < N; j++)
        {
            printf("%6.2f ", A[i][j]);
        }
        printf("]  [ x%d ] = [ %6.2f ]\n", i + 1, b[i]);
    }
    printf("\n");
}

int solveMatrix(double A[N][N], double b[N], double x[N])
{
    // Phase 1 & 2: Forward Elimination with Partial Pivoting
    for (int i = 0; i < N; i++)
    {

        // --- STEP 1: PARTIAL PIVOTING ---
        // Find the row with the largest absolute value in the current column
        int maxRow = i;
        for (int k = i + 1; k < N; k++)
        {
            if (fabs(A[k][i]) > fabs(A[maxRow][i]))
            {
                maxRow = k;
            }
        }

        // Swap the current row with the maxRow (both in Matrix A and Vector b)
        if (maxRow != i)
        {
            // Swap rows in A
            for (int k = i; k < N; k++)
            {
                double temp = A[i][k];
                A[i][k] = A[maxRow][k];
                A[maxRow][k] = temp;
            }
            // Swap elements in b
            double temp = b[i];
            b[i] = b[maxRow];
            b[maxRow] = temp;
        }

        // Check if the matrix is singular (e.g., floating wires or short circuits)
        if (fabs(A[i][i]) < EPSILON)
        {
            return -1; // Error: Matrix cannot be solved
        }

        // --- STEP 2: FORWARD ELIMINATION ---
        // Make all values below the current pivot element equal to 0
        for (int j = i + 1; j < N; j++)
        {
            double factor = A[j][i] / A[i][i];
            for (int k = i; k < N; k++)
            {
                A[j][k] -= factor * A[i][k];
            }
            b[j] -= factor * b[i];
        }
    }

    // --- STEP 3: BACK SUBSTITUTION ---
    // Solve the equations from the bottom row up to the top row
    for (int i = N - 1; i >= 0; i--)
    {
        double sum = 0.0;
        for (int j = i + 1; j < N; j++)
        {
            sum += A[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / A[i][i];
    }

    return 0; 
}

int main()
{
    // Hardcoded Modified Nodal Analysis (MNA) Matrix for our test circuit
    // Row 1 & 2 represent Kirchhoff's Current Law. Row 3 represents the Battery.
    double A[N][N] = {0.0};
    double b[N] = {0.0}; // Right hand side vector (Known values)
    double x[N] = {0.0}; // Solution vector where answers will be stored
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
        {
            scanf("%lf", &A[i][j]);
        }
    }
    for (int i = 0; i < N; i++)
    {

        scanf("%lf", &b[i]);
    }
    
    printf("--- Original Circuit MNA Matrix System ---\n");
    printSystem(A, b);

    // Run the solver
    int status = solveMatrix(A, b, x);

    if (status == 0)
    {
        printf("--- Simulation Results ---\n");
        printf("Voltage at Node 1: %6.2f V\n", x[0]);
        printf("Voltage at Node 2: %6.2f V\n", x[1]);
        printf("Current from Battery: %6.2f A\n", x[2]);
        printf("\n(Note: Negative current means it is leaving the positive terminal)\n");
    }
    else
    {
        printf("Simulation Error: Singular matrix detected. Check for floating nodes or illegal loops.\n");
    }

    return 0;
}
