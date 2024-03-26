//Protocolo do encaminhamento
#include "../hed/main.h"
#include "../hed/auxFunc.h"

void showRouting(int ***table, int dest)
{
    int destIndex = -1;
    
    printf("Tabela de encaminhamento para %d\n", dest);
    
    // Encontra o índice do destino na tabela
    for (int i = 0; i < MAX_NODES; i++)
    {
        if(table[i][DestinationIDsIndex] == NULL)
        {
            continue;
        }

        if (table[i][DestinationIDsIndex][0] == dest)
        {
            destIndex = i;
            break;
        }
    }

    if (destIndex == -1)
    {
        printf("%d Não é um destino\n", dest);
        return;
    }

    // Mostra a rota para o destino
    for (int i = 2; i < MAX_NEIGHBORSIndex; i++)
    {
        if (table[destIndex][i] == NULL)
        {
            continue;
        }
        if (table[destIndex][i][0] == -1)
        {
            printf("Pelo vizinho %d: ---\n", table[NeighborsIDsIndex][i][0]);
            continue;
        }
        
        printf("Pelo vizinho %d: ", table[DestinationIDsIndex][i][0]);
        for (int j = 0; j < MAX_NODES; j++)
        {
            if (table[destIndex][i][j] == dest)
            {
                printf("%d ", table[destIndex][i][j]);
                break;
            }
            printf("%d-", table[destIndex][i][j]);
        }
        printf("\n");
    }
}

void showPath(int ***tables, int dest)
{
    int destIndex = -1;
    
    printf("Caminho mais curto para %d\n", dest);
    
    // Encontra o índice do destino na tabela
    for (int i = 0; i < MAX_NODES; i++)
    {
        if(tables[i][DestinationIDsIndex] == NULL)
        {
            continue;
        }

        if (tables[i][DestinationIDsIndex][0] == dest)
        {
            destIndex = i;
            break;
        }
    }

    if (destIndex == -1)
    {
        printf("%d Não é um destino\n", dest);
        return;
    }

    if (tables[destIndex][SHORTESTPATH_INDEX] == NULL)
    {
        printf("Não há caminho para %d\n", dest);
        return;
    }

    // Mostra a rota para o destino
    for (int j = 0; j < MAX_NODES; j++)
    {
        if (tables[destIndex][SHORTESTPATH_INDEX][j] == dest)
        {
            printf("%d ", tables[destIndex][SHORTESTPATH_INDEX][j]);
            break;
        }
        printf("%d-", tables[destIndex][SHORTESTPATH_INDEX][j]);
    }
    printf("\n");
}

void showForwarding (int ***tables, int personal)
{
    printf("Expedição\n");
    printf("Destino |  Send To\n");
    printf("--------|---------\n");
    for (int i = 0; i < MAX_NODES; i++)
    {
        if (tables[i][DestinationIDsIndex] != NULL)
        {
            if (tables[i][DestinationIDsIndex][0] == personal)
            {
                printf("   %d    |    ---   \n", tables[i][DestinationIDsIndex][0]);
                continue;
            }
            printf("   %d    |    %d   \n", tables[i][DestinationIDsIndex][0], tables[i][NEXTSTEP_INDEX][0]);
        }
    }
}

void initPersonalpath(int ***tables, int personal)
{
    //colocar id do nó como destino
    tables[persRowIndex][DestinationIDsIndex]= calloc(1, sizeof(int));
    memoryCheck(tables[persRowIndex][DestinationIDsIndex]);

    tables[persRowIndex][DestinationIDsIndex][0]=personal;

    //colocar id do nó como "vizinho"
    tables[NeighborsIDsIndex][persColIndex]= calloc(1, sizeof(int));
    memoryCheck(tables[NeighborsIDsIndex][persColIndex]);

    tables[NeighborsIDsIndex][persColIndex][0]=personal;

    // atualizar o caminho para mim proprio
    tables[persRowIndex][persColIndex]= calloc(1, sizeof(int));
    memoryCheck(tables[persRowIndex][persColIndex]);

    tables[persRowIndex][persColIndex][0]=personal;

    //caminho mais curto para mim proprio
    tables[persRowIndex][SHORTESTPATH_INDEX]= tables[persRowIndex][persColIndex];

    //expedição para mim proprio
    tables[persRowIndex][NEXTSTEP_INDEX]= tables[persRowIndex][persColIndex];
}

void initTables(int ****tables)
{
    //camada de encaminhamento
    *tables = (int ***)calloc(NUMBER_OF_ROWS, sizeof(int **));
    memoryCheck((*tables));
    
    for(int i = 0; i < NUMBER_OF_ROWS; i++)
    {
        (*tables)[i] = (int **)calloc(NUMBER_OF_COLUMNS, sizeof(int *));
        memoryCheck((*tables)[i]);
    }
}

void freeTables(int ***tables)
{
    for (int i = 0; i < NUMBER_OF_ROWS; i++)
    {
        for (int j = 1; j < MAX_NEIGHBORSIndex; j++)
        {
            if (tables[i][j] != NULL)
            {
                free(tables[i][j]);
            }
        }
        free(tables[i]);
    }
    free(tables);
}

//inicializa destino com um array [-1, -1] em todos os vizinhos inicializados
void destinationInit(int ***table, int dest, int* indexDest)
{
    int i= 0;
    for (i = 2; i < MAX_DESTINATIONSIndex; i++)
    {
        if (table[i][DestinationIDsIndex] == NULL)
        {
            *indexDest= i;
            break;
        }
    }

    //colocar distancia para o novo destino com um vetor [-1, -1] para todos os vizinhos
    for  (int j = 2; j < MAX_NEIGHBORSIndex; j++)
    {
        if (table[NeighborsIDsIndex][j] != NULL)
        {
            table[i][j]= calloc(2, sizeof(int));
            memoryCheck(table[i][j]);

            table[i][j][0]= -1;
            table[i][j][1]= -1;

            printf("table[%d][%d][0]: %d\n", i, j, table[i][j][0]);
        }
    }

    //colocar o id do destino
    table[i][DestinationIDsIndex]= calloc(1, sizeof(int));
    memoryCheck(table[i][DestinationIDsIndex]);

    table[i][DestinationIDsIndex][0]= dest;
}

void neighbourInit(int ***table, int neighbour, int* indexNeigh)
{
    int i= 0;
    for (i = 2; i < MAX_NEIGHBORSIndex; i++)
    {
        if (table[NeighborsIDsIndex][i] == NULL)
        {
            *indexNeigh= i;
            break;
        }
    }

    //colocar o id do vizinho
    table[NeighborsIDsIndex][i]= calloc(1, sizeof(int));
    memoryCheck(table[NeighborsIDsIndex][i]);

    table[NeighborsIDsIndex][i][0]= neighbour;
}

//conversão de vetor de inteiros para string
void pathIntToString(int *path, char *string, int dest)
{
    int i = 0;
    char member[4];

    while (path[i] != dest)
    {
        sprintf(member, "%02d-", path[i]);
        strcat(string, member);
        i++;
        printf("string: %s\n", string);
    }
    
    sprintf(member, "%02d", path[i]);
    strcat(string, member);
}

//conversão de string para vetor de inteiros
//path é um array de inteiros que contem o caminho
void stringToPathInt(char* string, int **path, int *pathSize)
{
    char **stringSeperated= NULL;

    if (string == NULL)
    {
        *path= NULL;
        *pathSize= 0;
        return;
    }
    
    stringSeperated= (char **)calloc(MAX_ARGUMENTS, sizeof(char *));
    memoryCheck(stringSeperated);

    messageTokenize(string, stringSeperated, pathSize, 2);

    printf("pathSize: %d\n", *pathSize);
    for (int i = 0; i < (*pathSize); i++)
    {
        printf("stringSeperated[%d]: %s\n", i, stringSeperated[i]);
    }

    //para depois poder adicionar o id do nó pessoal
    (*pathSize)++;

    (*path)= (int *)calloc((*pathSize), sizeof(int));
    memoryCheck(*path);

    for (int i= 0; i < (*pathSize)-1; i++)
    {
        (*path)[i+1]= atoi(stringSeperated[i]);
        printf("path[%d]: %d\n", i, (*path)[i+1]);
    }

    for (int i = 0; i < (*pathSize); i++)
    {
        free(stringSeperated[i]);
    }
    free(stringSeperated);
}

//compara o shortest path antigo com o nomeado a novo shortes path indica se o novo é mais curto ou não
//retorna 1 se for mais curto
//        0 se for igual ou maior
//       -1 se o antigo for NULL    
int shortestPathCalc(int *old, int *new, int dest)
{
    int i = 0;
        printf("old é NUddddgLL\n");
    if (old == NULL)
    {
        return 1;
    }
        printf("old é gggggg\n");
    while (old[i] != dest || new[i]  != dest)
    {
        i++;
    }
        printf("old é NhhhhhULL\n");
    if (old[i] == dest)
    {
        return 0;
    }

    if (new[i]==dest)
    {
        return 1;
    }

    return -1;
}

//last to columns of the table are the shortest path and next step tables
void shortestPath_nextStep_update()
{

}

void routingRecive(int ***tables, char* pathString, int neighbour, int destination, int personalId)
{
    //mensagem em inteiros
    int indexNeigh = -1, indexDest = -1;

    int *intengerPath = NULL;
    int integerPathSize= 0;

    //index's da tabela do neighbour e da destination
    for (int i = 2; i<MAX_NEIGHBORSIndex; i++)
    {
        //neighbour
        if (tables[NeighborsIDsIndex][i]!=NULL)
        {
            if (i!= MAX_NEIGHBORSIndex)
            {
                if (tables[NeighborsIDsIndex][i][0]==neighbour)
                {
                    indexNeigh = i;
                }
            }
        }

        //destination
        if (tables[i][DestinationIDsIndex]!=NULL)
        {
            if (tables[i][DestinationIDsIndex][0]==destination)
            {
                indexDest = i;
            }
        }
    }

    //new destination?
    if (indexDest == -1)
    {
        destinationInit(tables, destination, &indexDest);
    }
    if (indexNeigh == -1)
    {
        neighbourInit(tables, neighbour, &indexNeigh);
    }

    //Separar o caminho
    stringToPathInt(pathString, &intengerPath, &integerPathSize);

    // verificar se existe caminho
    if(intengerPath != NULL)
    {
        //existe caminho podemos concatenar o nosso personal.id ao caminho a partir do vizinho
        intengerPath[0] = personalId;
    }

    for (int i = 0; i < integerPathSize; i++)
    {
        printf("intengerPath[%d]: %d\n", i, intengerPath[i]);
    }

    //esta alteração muda o shortestPath?
    if (shortestPathCalc(tables[indexDest][SHORTESTPATH_INDEX], intengerPath, destination) == 1)
    {
        printf("Caminho mais curto\n");

        //colocar o path na tabela [destination.Id] [neighbour.id]
        free(tables[indexDest][indexNeigh]);
        tables[indexDest][indexNeigh]= intengerPath;

        showRouting(tables, destination);

        // //colocar o novo caminho mais curto
        // tables[indexDest][SHORTESTPATH_INDEX]= tables[indexDest][indexNeigh];
        // tables[indexDest][NEXTSTEP_INDEX]= tables[indexDest][indexNeigh][1];
    }
    else
    {
        free(intengerPath);
    }
}

void routingMessageCreate(int ***tables, char** message, int destIndex)
{
    //Criar a mensagem a enviar para os vizinhos
    char *string = (char *)calloc(MAX_BUFFER, sizeof(char));    
    memoryCheck(string);

    //conversão do caminho para string
    pathIntToString(tables[destIndex][SHORTESTPATH_INDEX], string, tables[destIndex][DestinationIDsIndex][0]);

    //criar a mensagem
    (*message)= (char *)calloc(MAX_BUFFER, sizeof(char));
    memoryCheck(*message);

    sprintf((*message), "ROUTE %02d %02d %s\n", tables[persRowIndex][DestinationIDsIndex][0], tables[destIndex][DestinationIDsIndex][0], string);

    free(string);
}

void routingSendAll(int ***tables, int neighbourFd)
{
    char* message= NULL;

    for (int i = 1; i < NUMBER_OF_ROWS - 1; i++)
    {
        if (tables[i][DestinationIDsIndex] != NULL)
        {
            //alocar memoria para a mensagem
            message = (char *)calloc(MAX_BUFFER, sizeof(char));
            memoryCheck(message);

            //criar a mensagem
            routingMessageCreate(tables, &message, i);

            tcpSend(neighbourFd, message);
            printf("Mensagem enviada: %s\n", message);

            free(message);
        }
    }
}
    

void newAdjacency()
{

}

void removeAdjacency()
{

}



