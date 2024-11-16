#include "MKTMath.h"

void multiplyMatrices(double complex mat1[][3], int row1, int col1, double complex mat2[][2], int row2, int col2, double complex result[][2])
{
    int x, y, z;
    // Performs matrix multiplication
    for (x = 0; x < row1; x++)
    {
        for (y = 0; y < col2; y++)
        {
            result[x][y] = 0;
            for (z = 0; z < col1; z++)
            {
                //ta errado
                result[x][y] += mat1[x][z] * mat2[z][y];
            }
        }
    }
}

void showMatrix(int row, int col, double complex mat[row][col])
{
    int x, y;

    printf("Resultant Matrix: (%dx%d)\n", row, col);
    for (x = 0; x < row; x++)
    {
        for (y = 0; y < col; y++)
        {
            printf("%lf + i%lf\t", creal(mat[x][y]), cimag(mat[x][y]));
        }
        printf("\n");
    }
}

void getFinalMatrix(
    int matrixRange,
    double H[matrixRange - 1][matrixRange - 1],
    double N[matrixRange - 1][matrixRange - 2],
    double J[matrixRange - 2][matrixRange - 1],
    double L[matrixRange - 2][matrixRange - 2],
    double matrizJacobiana[matrixRange + 1][matrixRange + 1])
{
    int i = 0;
    int j = 0;
    int rangeH = 3;
    for (i = 0; i < matrixRange; i++)
    {
        for (j = 0; j < matrixRange; j++)
        {
            if (j >= rangeH)
            {

                matrizJacobiana[i][j] = N[i][j - rangeH];
            }
            else
            {
                matrizJacobiana[i][j] = H[i][j];
            }
            printf("%d:%d \t", i, j);
        }
        printf("\n");
    }
}

void printAdmittanceMatrix(int matrixRange, double complex Y[matrixRange][matrixRange])
{
    int i = 0;
    int j = 0;
    for (i = 0; i < matrixRange; i++)
    {
        for (j = 0; j < matrixRange; j++)
        {
            printf("%.2lf + %.2lfi\t", creal(Y[i][j]), cimag(Y[i][j]));
        }
        printf("\n");
    }
}

void getIdentityMatrix(int length, double complex mat[length][length])
{
    int i = 0;
    int j = 0;

    for (i = 0; i < length; i++)
    {
        for (j = 0; j < length; j++)
        {
            if (i == j)
            {
                mat[i][j] = 1;
            }
            else
            {
                mat[i][j] = 0;
            }
        }
    }
}

double complex get2x2Determinant(double complex mat[2][2])
{
    double complex result = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    printf("returned %lf + i%lf\n", creal(result), cimag(result));
    return result;
}

void cutMatrix(int row, int col, double complex mat[row][col], int rowCut, int colCut, double complex resultantMatrix[row - 1][col - 1])
{

    int i = 0;
    int j = 0;
    int rowcut_count = 0;
    int colcut_count = 0;

    double complex **result;

    for (i = 0; i < row - 1; i++)
    {   
        rowcut_count = 0;
        if (i >= rowCut)
        {
            rowcut_count = 1;
        }
        colcut_count = 0;
        for (j = 0; j < col - 1; j++)
        {
            if (j >= colCut)
            {
                colcut_count = 1;
            }
            resultantMatrix[i][j] = mat[i + rowcut_count][j + colcut_count];
        }
    }
}

double complex getDeterminant(int size, double complex mat[size][size])
{
    double complex matrix[size - 1][size - 1];
    double complex aux[size - 1][size - 1];
    if (size > 2)
    {
        // separar e obter o tamanho!
        int k = 0;
        int l = 0;
        for (k = 0; k < size ; k++)
        {
            for (l = 0; l < size; l++)
            {
                cutMatrix(size, size, mat, k, l, aux);
                printf("(%d:%d) => k=%d l=%d\n", size, size, k, l);
                double complex det = getDeterminant(size - 1, aux);
                matrix[k][l] = pow(-1, k + l) * mat[k][l]* det;
            }
        }
    }
    else
    {
        showMatrix(2, 2, mat);
        return get2x2Determinant(mat);
    }
}

void inverse_matrix(int row, int col, double complex mat[row][col], double complex inv[row][col])
{
}