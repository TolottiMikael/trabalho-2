#ifndef matematica_h
#define matematica_h

#include <complex.h>
#include <math.h>



void multiplyMatrices(double complex mat1[][3], int row1, int col1, double complex mat2[][2], int row2, int col2, double complex result[][2]);
void showMatrix(int row, int col, double complex mat[row][col]);
void getFinalMatrix(int matrixRange, double H[matrixRange - 1][matrixRange - 1], double N[matrixRange - 1][matrixRange - 2], double J[matrixRange - 2][matrixRange - 1], double L[matrixRange - 2][matrixRange - 2], double matrizJacobiana[matrixRange + 1][matrixRange + 1]);
void printAdmittanceMatrix(int matrixRange, double complex Y[matrixRange][matrixRange]);
void getIdentityMatrix(int length, double complex mat[length][length]);
double complex get2x2Determinant(double complex mat[2][2]);
void cutMatrix(int row, int col, double complex mat[row][col], int rowCut, int colCut, double complex resultantMatrix[row-1][col-1]);
double complex getDeterminant(int size,double complex mat[size][size]);
double complex getCofactorMatrix(int size,double complex mat[size][size]);
void inverse_matrix(int row, int col, double complex mat[row][col], double complex inv[row][col]);

#include "MKTMath.c"
#endif