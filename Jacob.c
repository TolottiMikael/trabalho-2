#include <stdio.h>
#include "../circuit/circuit.h"
#include "../mikaelMath/MKTMath.h"



int main()
{
    printf("Este é um script em C que realiza a criação de uma tabela de Admitâncias\n");
    printf("Digite as impedâncias e reatâncias das linhas\n");
    printf("Separe as ligações de linhas com enter\n");
    printf("Digite FIM para encerrar\n");

    float Vbase = 230.0000000000000000;
    float Sbase = 100.0000000000000000;
    printf("Vbase: %f\n", Vbase);
    printf("Sbase: %f\n", Sbase);

    char linha[100];
    circuitBar *circuitos = NULL;
    int count = 0;
    int i = 0;
    int j = 0;
    int k = 0;
    int l = 0;

    conexoesBar *conexoes = malloc(1 * sizeof(conexoesBar));
    conexoes->numCircuitos = 0;
    conexoes->partida = NULL;
    conexoes->destino = NULL;
    int numConex = 0;

    // string para coletar entrada do usuário
    char input[100];

    do
    {
        printf("Entrada: ");
        scanf("%99s", linha);

        circuitBar novoCircuito;
        // tentando verificar a exitência de um circuito
        if (sscanf(linha, "%d-%d:%lf;%lf;%lf",
                   &novoCircuito.partida, &novoCircuito.destino,
                   &novoCircuito.impedancia, &novoCircuito.reatancia,
                   &novoCircuito.power) != 5)
        {
            printf("Entrada não é um circuito\n");
        }
        else
        {

            novoCircuito.partida--;
            novoCircuito.destino--;

            // partida é sempre menor que destino
            if (novoCircuito.partida > novoCircuito.destino)
            {
                int aux = novoCircuito.partida;
                novoCircuito.partida = novoCircuito.destino;
                novoCircuito.destino = aux;
            }

            // verificando as conexões
            int conexIndex = getExistingConnection(conexoes, novoCircuito.partida, novoCircuito.destino);

            if (conexIndex == -1)
            {
                // adicionando uma conexão nova
                conexoes->numCircuitos++;
                conexoes->partida = realloc(conexoes->partida, conexoes->numCircuitos * sizeof(int));
                conexoes->destino = realloc(conexoes->destino, conexoes->numCircuitos * sizeof(int));
                conexoes->partida[conexoes->numCircuitos - 1] = novoCircuito.partida;
                conexoes->destino[conexoes->numCircuitos - 1] = novoCircuito.destino;

                circuitos = realloc(circuitos, (count + 1) * sizeof(circuitBar));
                circuitos[count++] = novoCircuito;
            }
            else
            {
                printf("este circuito já Existe!\n");
            }
        }
    } while (strcmp(linha, "FIM") != 0);

    printConnAndCircuit(circuitos, conexoes, count);

    printf("count : %d\n", count);
    int matrixRange = getMaiorBar(circuitos, count) + 1;
    printf("matrixRange : %d\n", matrixRange);
    // calculando admitâncias
    double complex y[matrixRange][matrixRange];
    double complex Y[matrixRange][matrixRange];

    for (i = 0; i < matrixRange; i++)
    {
        for (j = 0; j < matrixRange; j++)
        {
            if (i == j)
            {
                // diagonal principal
                y[i][j] = 0;
                conexoesBar conexoes_da_barra = getConexoesBetween(i, conexoes);
                int k = 0;
                for (k = 0; k < conexoes_da_barra.numCircuitos; k++)
                {
                    circuitBar circuito_ = getCircuitoBetween(conexoes_da_barra.partida[k], conexoes_da_barra.destino[k], circuitos, count);
                    y[i][j] += (circuito_.power / Sbase) / 2.00000;
                    // if (i == 0)
                    // {
                    //     printf("y0 = %lf \n ", y[i][j]);
                    //     printf("%lf\n", circuito_.power);
                    // }

                    y[i][j] += 1 / (circuito_.impedancia + (I * circuito_.reatancia));

                    // if (i == 0)
                    // {
                    //     printf("y0 = %lf \n ", y[i][j]);
                    //     printf("%lf\n",  );
                    // }
                }
                Y[i][j] = y[i][j];
            }
            else
            {
                circuitBar circuit = getCircuitoBetween(i, j, circuitos, count);
                if (isNullCircuit(circuit))
                {
                    y[i][j] = 0;
                }
                else
                {
                    y[i][j] = 1 / (circuit.impedancia + (I * circuit.reatancia));
                }
                Y[i][j] = -y[i][j];
            }
        }
    }

    printf("Matriz de Admitâncias y \n");
    printAdmittanceMatrix(matrixRange, y);

    
    printf("\n\nMatriz de Admitâncias y \n");
    printAdmittanceMatrix(matrixRange, Y);
    printf("\n");
    printf("\n");

    // calculando as tensoes e correntes
    nodesValuesBar nodes[2][matrixRange];
    unsigned int interaction = 0;
    double complex erro[matrixRange];

    // não utilizado mais dessa forma!
    for (i = 0; i < matrixRange; i++)
    {
        for (j = 0; j < matrixRange; j++)
        {
            printf("%lf <%lf\t\t", cabs(Y[i][j]), carg(Y[i][j]) * RAD_TO_DEG);

            // utilizado mais a frente
            nodes[0][i].voltage = 1;
            nodes[0][i].real_Power = 1;
            nodes[0][i].reactive_Power = 1;
            nodes[0][i].apparent_Power = 1;
            nodes[0][i].delta = 0;

            nodes[1][i].voltage = 1;
            nodes[1][i].real_Power = 1;
            nodes[1][i].reactive_Power = 1;
            nodes[1][i].apparent_Power = 1;
            nodes[1][i].delta = 0;

            nodes[0][i].isSlackbar = 0;
            nodes[0][i].doesHaveGenerator = 0;

            nodes[1][i].isSlackbar = 0;
            nodes[1][i].doesHaveGenerator = 0;

            erro[i] = 0;
        }
        printf("\n");
    }

    printf("para Prosseguir você deve indicar informações importantes\n");
    printf("\n\n <==> \n\n");
    printf("indique qual barra é slack\n");
    int inputs = 1;
    scanf("%d", &inputs);
    for (i = 0; i < 2; i++)
    {
        nodes[i][inputs - 1].isSlackbar = 1;
        nodes[i][inputs - 1].doesHaveGenerator = 1;
    }
    printf("barra %d  \n", nodes[1][0].isSlackbar);
    printf("barra %d  \n", nodes[1][1].isSlackbar);

    double value;
    for (i = 0; i < matrixRange; i++)
    {
        printf("qual a carga ativa na barra %d\n", i + 1);
        scanf("%lf", &value);
        nodes[0][i].carga.ativa = value;
        nodes[1][i].carga.ativa = value;

        printf("qual a carga reativa na barra %d\n", i + 1);
        scanf("%lf", &value);
        nodes[0][i].carga.reativa = value;
        nodes[1][i].carga.reativa = value;

        printf("qual a geração em MW na barra (digite 0 se não ter gerador) %d\n", i + 1);
        scanf("%lf", &value);
        nodes[0][i].generator = value;
        nodes[1][i].generator = value;
        if (value == 0)
        {
            nodes[0][i].doesHaveGenerator = 0;
            nodes[1][i].doesHaveGenerator = 0;
        }
        else
        {
            nodes[0][i].doesHaveGenerator = 1;
            nodes[1][i].doesHaveGenerator = 1;
        }
    }

    char string[1000] = "";
    // printNodesArray(nodes[0], nodes[1], matrixRange, string);
    // printf("%s\n", string);
    double deltaP[matrixRange];
    double deltaQ[matrixRange];

    int MAXInteration = 1;

    do
    {
        attNodeValues(nodes[1], nodes[0], matrixRange, Y, conexoes, circuitos);
        interaction++;

        printf(" <==================> \n nós após a interação\n");

        strcpy(string, "");
        printNodesArray(nodes[0], nodes[1], matrixRange, string);
        printf("%s\n", string);

        // calculando os resíduos

        for (i = 0; i < matrixRange; i++)
        {
            deltaP[i] = ((nodes[1][i].generator - nodes[1][i].carga.ativa) / Sbase) - nodes[1][i].real_Power;
            deltaQ[i] = ((nodes[1][i].generator - nodes[1][i].carga.reativa) / Sbase) - nodes[1][i].reactive_Power;
        }

        double H[matrixRange - 1][matrixRange - 1];
        double J[matrixRange - 2][matrixRange - 1];
        double N[matrixRange - 1][matrixRange - 2];
        double L[matrixRange - 2][matrixRange - 2];

        int countSlackBarROW = 0;
        int countSlackBarCOL = 0;

        for (i = 0; i < matrixRange; i++)
        {
            if (nodes[1][i].isSlackbar == 0)
            {
                for (j = 0; j < matrixRange; j++)
                {
                    if (nodes[1][j].isSlackbar == 0)
                    {
                        if (i == j)
                        {
                            // diagonal principal
                            H[i - countSlackBarROW][j - countSlackBarCOL] = -(nodes[1][i].reactive_Power) -
                                                                            (pow(cabs(nodes[1][i].voltage), 2) * cimag(Y[i][j]));
                            // restrições para N
                            if (nodes[1][j].doesHaveGenerator != 1)
                            {
                                // não tem gerador
                                N[i - countSlackBarROW][j - countSlackBarCOL] = nodes[1][i].real_Power +
                                                                                (pow(cabs(nodes[1][i].voltage), 2)) * creal(Y[i][j]);
                            }

                            // condições para J
                            if (nodes[1][i].doesHaveGenerator != 1)
                            {
                                J[i - countSlackBarROW][j - countSlackBarCOL] = nodes[1][i].real_Power -
                                                                                (pow(cabs(nodes[1][i].voltage), 2)) * creal(Y[i][j]);
                            }

                            // Condições para L
                            if (
                                (nodes[1][i].doesHaveGenerator != 1) &&
                                (nodes[1][j].doesHaveGenerator != 1))
                            {
                                L[i - countSlackBarROW][j - countSlackBarCOL] = nodes[1][i].reactive_Power -
                                                                                (pow(cabs(nodes[1][i].voltage), 2) * cimag(Y[i][j]));
                            }
                        }
                        else
                        {
                            // diagonal não principal
                            H[i - countSlackBarROW][j - countSlackBarCOL] = -((cabs(nodes[1][i].voltage) * cabs(nodes[1][j].voltage) * cabs(Y[i][j])) * sin(((carg(Y[i][j]) * RAD_TO_DEG) + nodes[1][j].delta - nodes[1][i].delta) * DEG_TO_RAD));
                            if (nodes[1][j].doesHaveGenerator != 1)
                            {
                                // não tem gerador
                                N[i - countSlackBarROW][j - countSlackBarCOL] = cabs(cabs(nodes[1][i].voltage) * cabs(nodes[1][j].voltage) * cabs(Y[i][j])) * cos(((carg(Y[i][j]) * RAD_TO_DEG) - nodes[1][j].delta + nodes[1][i].delta) * DEG_TO_RAD);
                            }

                            // condições para J
                            if (nodes[1][i].doesHaveGenerator != 1)
                            {
                                J[i - countSlackBarROW][j - countSlackBarCOL] = cabs(cabs(nodes[1][i].voltage) * cabs(nodes[1][j].voltage) * cabs(Y[i][j])) * cos(((carg(Y[i][j]) * RAD_TO_DEG) - nodes[1][j].delta + nodes[1][i].delta) * DEG_TO_RAD);
                            }

                            // Condições para L
                            if (
                                (nodes[1][i].doesHaveGenerator != 1) &&
                                (nodes[1][j].doesHaveGenerator != 1))
                            {
                                L[i - countSlackBarROW][j - countSlackBarCOL] = -cabs(cabs(nodes[1][i].voltage) * cabs(nodes[1][j].voltage) * cabs(Y[i][j])) *
                                                                                sin(((carg(Y[i][j]) * RAD_TO_DEG) + nodes[1][j].delta - nodes[1][i].delta) * DEG_TO_RAD);
                            }
                        }
                        // printf("nope pulado %d\n", j);
                        // printf("H[%d][%d] = %lf \t", i - countSlackBarROW, j - countSlackBarCOL, H[i - countSlackBarROW][j - countSlackBarCOL]);
                    }
                    else
                    {
                        // printf("pulado a coluna %d\n", j);
                        countSlackBarCOL++;
                    }
                }
                printf("\n");
                countSlackBarCOL = 0;
            }
            else
            {
                // printf("pulado a linha %d\n", i);
                countSlackBarROW++;
            }
        }

        double matrizJacobiana[matrixRange + 1][matrixRange + 1];

        getFinalMatrix(matrixRange + 1, H, N, J, L, matrizJacobiana);
        showMatrix(5, 5, matrizJacobiana);

    } while (interaction < MAXInteration);

    // desalocando memória
    free(conexoes->partida);
    free(conexoes->destino);
    free(conexoes);
    free(circuitos);
}