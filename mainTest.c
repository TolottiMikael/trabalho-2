#include <stdio.h>
#include <complex.h>
#include <math.h>

#include "./mikaelMath/MKTMath.h"

int main()
{
    // double complex identityMatrix[2][2];
    // getIdentityMatrix(2, identityMatrix);
    // showMatrix(2, 2, identityMatrix);

    // double complex matrix[2][2] = {{1, 2}, {3, 4}};
    // printf("Determinante: %lf + i%lf\n", creal(get2x2Determinant(matrix)), cimag(get2x2Determinant(matrix)));

    double complex matrix3x3[3][3] = {
        {1 + 1I, 2 + 2I, 3 + 3I},
        {4 + 4I, 5 + 5I, 6 + 6I},
        {7 + 7I, 8 + 8I, 9 + 9I}};
    double complex matrix3x3Real[3][3] = {
        {1, 0, 2},
        {6, 8, 9},
        {6, 5, 10}};
    double complex matrix5x5Real[5][5] = {
        {1, 0, 2, 3, 5},
        {1, 1, 1, 1, 1},
        {-6, 0, 0, 0, 4},
        {5, 3, 3, 3, 3},
        {0, 78, 23, 46, 4}};

    double complex matrix4x4Real[4][4] = {
        {1, 0, 4, -6},
        {2, 5, 0, 3},
        {-1, 2, 3, 5},
        {2, 1, -2, 3}};

    // double complex determinant3x3 = getDeterminant(3, matrix3x3);
    // printf("det 3x3: %lf + i%lf\n", creal(determinant3x3), cimag(determinant3x3));

    // double complex determinant5x5 = getDeterminant(5, matrix5x5Real);
    // printf("det 5x5: %lf + i%lf\n", creal(determinant5x5), cimag(determinant5x5));

    // double complex determinant3x3Real = getDeterminant(3, matrix3x3Real);
    // printf("det 3x3 só real: %lf + i%lf\n", creal(determinant3x3Real), cimag(determinant3x3Real));

    // double complex determinant4x4Real = getDeterminant(4, matrix4x4Real);
    // printf("det 4x4 só real: %lf + i%lf\n", creal(determinant4x4Real), cimag(determinant4x4Real));

    double complex inverse[5][5];

    getInverse_matrix(5, matrix5x5Real, inverse);


    showMatrix(5,5, inverse);
    return 0;
}