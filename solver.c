#include <math.h>
#include "solver.h"

#define EPSILON 1e-9

int solveMatrixDynamic(
    double A[MAX_MNA][MAX_MNA],
    double b[MAX_MNA],
    double x[MAX_MNA],
    int size)
{
    if (size <= 0 || size > MAX_MNA)
    {
        return -1;
    }

    // Forward elimination with partial pivoting
    for (int i = 0; i < size; i++)
    {
        int maxRow = i;

        for (int k = i + 1; k < size; k++)
        {
            if (fabs(A[k][i]) > fabs(A[maxRow][i]))
            {
                maxRow = k;
            }
        }

        if (maxRow != i)
        {
            for (int k = i; k < size; k++)
            {
                double temp = A[i][k];
                A[i][k] = A[maxRow][k];
                A[maxRow][k] = temp;
            }

            double temp = b[i];
            b[i] = b[maxRow];
            b[maxRow] = temp;
        }

        // Singular or nearly singular matrix
        if (fabs(A[i][i]) < EPSILON)
        {
            return -1;
        }

        for (int j = i + 1; j < size; j++)
        {
            double factor = A[j][i] / A[i][i];

            for (int k = i; k < size; k++)
            {
                A[j][k] -= factor * A[i][k];
            }

            b[j] -= factor * b[i];
        }
    }

    // Back substitution
    for (int i = size - 1; i >= 0; i--)
    {
        double sum = 0.0;

        for (int j = i + 1; j < size; j++)
        {
            sum += A[i][j] * x[j];
        }

        x[i] = (b[i] - sum) / A[i][i];
    }

    return 0;
}