// Protocolo do encaminhamento
#include "../hed/main.h"

// receber informação dos vizinhos
bool personalInPathCheck(int destination, int *pathArray, int personal)
{
    for (int i = 1; pathArray[i] = destination, i++)
    {
        if (pathArray[i - 1] == personal)
        {
            return true;
        }
    }
    return false;
}

// atualizar as tabelas
void updateTables(int origin, int destination, char *path, int personal)
{
    int pathSize = 0;
    pathSize = sizeof(pathArray) / 2;
    char *path = ArrayToString(pathArray, pathSize, personal)

    // Atualizar a tabela de encaminhamento
    strcpy(RoutingTable[origin][destination], path);

    // Atualizar a tabela de caminhos mais curtos
    int minHyphens = -1;
    char *minPath = NULL;
    int hyphens = 0;
    for (int i = 0; i < size; i++)
    {
        hyphens = countHyphens(RoutingTable[i]);

        if (minHyphens == -1 || hyphens < minHyphens)
        {
            minHyphens = hyphens;
        }

        minPath = RoutingTable[i];
    }

    if (minPath != ShortestPathTable[destination])
    {
        strcpy(ShortestPathTable[destination], minPath);
    }

    // Atualizar a tabela de expedição
    int expPath = 0;
    int count = 0;
    char secondNum[3] = {0};

    // Encontrar e extrair o segundo número da string minPath
    for (int i = 0; minPath[i] != '\0'; i++)
    {
        if (minPath[i] == '-')
        {
            count++;
            if (count == 1)
            { // Se é o primerio "-", o proximo numero é o segundo numero
                // Extrair os dois digitos a seguir ao "-"
                sprintf(secondNum, "%c%c", minPath[i + 1], minPath[i + 2]);
                // converter para inteiro
                expPath = atoi(secondNum);
                break; // Sai do for quando encontrar o segundo numero
            }
        }
    }

    strcpy(ExpeditionTable[destination], expPath);
}

void printRoutingTable(int personal)
{
    for (int i = 0; i < MAX_IDS; i++)
    {
        for (int j = 0; j < MAX_IDS; j++)
        {
            if (strlen(RoutingTable[i][j]) == 0)
            {
                printf("Route from %d to %d by %d: -\n", personal, j, i);
            }
            else
            {
                printf("Route from %d to %d by %d: %s\n", personal, j, i, RoutingTable[i][j]);
            }
        }
    }
}

void printShortestPathTable()
{
    for (int i = 0; i < MAX_IDS; i++)
    {

        printf("Shortest path to %d: %s\n", i, ShortPathTable[i]);
    }
}

void printExpeditionTable()
{
    for (int i = 0; i < MAX_IDS; i++)
    {
        if (ExpeditionTable[i] == -1)
        {
            printf("Expedition to %d: -\n", i);
        }
        else
        {
            printf("Expedition to %d: %d\n", i, ExpeditionTable[i]);
        }
    }
}

// // Enviar informações de encaminhamento para os vizinhos
void sendRoutingInfo(int origin, int destination, int *pathArray)
{
}
