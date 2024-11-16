#include <stdio.h>
#include <complex.h>
#include <math.h>

#include "mikaelMath\MKTMath.h"

int main()
{

    // double complex identityMatrix[2][2];
    // getIdentityMatrix(2, identityMatrix);
    // showMatrix(2, 2, identityMatrix);

    // double complex matrix[2][2] = {{1, 2}, {3, 4}};
    // printf("Determinante: %lf + i%lf\n", creal(get2x2Determinant(matrix)), cimag(get2x2Determinant(matrix)));

    double complex mat[5][5] = {
        {1 + 1*I, 0 + 1*I, 0, 0, 0},
        {1, 1 + 1*I, 0 + 1*I, 0, 0},
        {0, 1, 1 + 1*I, 0 + 1*I, 0},
        {0, 0, 1, 1 + 1*I, 0 + 1*I},
        {0, 0, 0, 1, 1 + 1*I}
    };

    // showMatrix(5, 5, mat);
    // double complex resultantMatrix[4][4];
    // cutMatrix(5, 5, mat, 2, 2, resultantMatrix);
    // showMatrix(4, 4, resultantMatrix);
    // double complex determinant = getDeterminant(5, mat);

    double complex matrix3x3[3][3] = {
        {1 + 1I, 2 + 2I, 3 + 3I},
        {4 + 4I, 5 + 5I, 6 + 6I},
        {7 + 7I, 8 + 8I, 9 + 9I}
    };

    double complex determinant3x3 = getDeterminant(4, mat);

    printf("det 3x3: %lf + i%lf\n", creal(determinant3x3), cimag(determinant3x3));
    return 0;
}