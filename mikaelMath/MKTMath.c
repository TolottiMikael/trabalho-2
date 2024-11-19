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
                // ta errado
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
    matriz_Jacobiana matrizContainer)
{
    int i = 0;
    int j = 0;
    int m = 0;

    double complex aux[matrixRange][matrixRange];
    

    for (i = 0; i < matrizContainer.rangeH; i++)
    {
        for (j = 0; j < matrizContainer.rangeH; j++)
        {
            // printf("(%d,%d)\t", i, j);

            aux[i][j] = matrizContainer.H[i][j];
        }
        for (m = matrizContainer.rangeH; m < matrixRange; m++)
        {
            // printf("(%d,%d)\t", i, m);

            aux[i][m] = matrizContainer.N[i][m];
        }
        // printf("\n");
    }
    for (i = 0; i < matrixRange - matrizContainer.rangeH; i++)
    {
        for (j = 0; j < matrizContainer.rangeH; j++)
        {
            // printf("(%d,%d)\t", i, j);
            aux[i + matrizContainer.rangeH][j] = matrizContainer.J[i][j];
        }
        for (m = matrizContainer.rangeH; m < matrixRange; m++)
        {
            // printf("(%d,%d)\t", i, m);
            aux[i][m] = matrizContainer.L[i][m - matrizContainer.rangeH];
        }
        // printf("\n");
    }
    matrizContainer.Jacobiana = (double complex(**))aux;
    showMatrix(matrixRange, matrixRange, (double complex(*)[matrixRange])matrizContainer.Jacobiana);
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

double complex get2x2Determinant(double complex mat[2][2])
{
    double complex result = mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
    // printf("returned %lf + i%lf\n", creal(result), cimag(result));
    return result;
}

double complex getDeterminant(int size, double complex mat[size][size])
{
    double complex aux[size - 1][size - 1];
    double complex value = 0;
    if (size > 2)
    {
        // separar e obter o tamanho!
        int l = 0;
        for (l = 0; l < size; l++)
        {
            cutMatrix(size, size, mat, 0, l, aux);
            // printf("(%d:%d) => k=%d l=%d\n", size, size, 0, l);
            double complex det = getDeterminant(size - 1, aux);
            value += pow(-1, 0 + l) * det * mat[0][l];
            // printf("um response  :  %lf + i%lf\n", creal(value), cimag(value));
        }
        return value;
    }
    else
    {
        // showMatrix(2, 2, mat);
        return get2x2Determinant(mat);
    }
}

void getCofactorMatrix(int size, double complex mat[size][size], double complex result[size][size])
{
    double complex aux[size - 1][size - 1];
    int i = 0;
    int j = 0;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            cutMatrix(size, size, mat, i, j, aux);
            result[i][j] = pow(-1, i + j) * getDeterminant(size - 1, aux);
        }
    }
}
void getTranspose(int size, double complex mat[size][size], double complex result[size][size])
{
    int i = 0;
    int j = 0;
    for (i = 0; i < size; i++)
    {
        for (j = 0; j < size; j++)
        {
            result[i][j] = mat[j][i];
        }
    }
}

// devo resolver o bug da inversão
void getInverse_matrix(int size, double complex mat[size][size], double complex inv[size][size])
{
    double complex determinant = getDeterminant(size, mat);
    double complex cofactor[size][size];
    double complex adjoint[size][size];
    getCofactorMatrix(size, mat, cofactor);
    // printf("cofatores       \n");
    // showMatrix(size, size, cofactor);
    getTranspose(size, cofactor, adjoint);
    // printf("matriz adjunta\n");
    // showMatrix(size, size, adjoint);
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            inv[i][j] = adjoint[i][j] / determinant;
        }
    }
}