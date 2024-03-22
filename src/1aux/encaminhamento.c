#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <main.h>

#define MAX_NODES 100
#define MAX_Arr_LENGTH 256

// Funcoes auxiliares

char* ArrayToString(int Arr[], int size, int personal) {
    // Calcular o comprimento da string final
    int total_length = size * 3 ; // 2 digitos + 1 separador '-' para cada elemento, mais 1 para o terminador nulo
    char* result = malloc(total_length * sizeof(char));
    if (result == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    // inicializar a string result
    snprintf(result, sizeof(result), "%d-", personal);

    // iterar sobre o Array e adicionar cada elemento à string result
    for (int i = 0; i < size; i++) {
        // Converter o inteiro para uma string
        char num_str[3]; // Assumindo que cada id tem no máximo dois digitos
        snprintf(num_str, sizeof(num_str), "%02d", Arr[i]); // verifica que cada numero é representado com pelo menos 2 digitos

        // adicionar a representação em string do numero ao resultado
        strcat(result, num_str);

        // adicionar um separador "-" a menos que seja o ultimo elemento	
        if (i < size - 1) {
            strcat(result, "-");
        }
    }

   
    return result;
}

//contar o numero de hifens que é porporcional ao comnpriemnto do caminho
int countHyphens(const char *str) {
    int count = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (str[i] == '-') {
            count++;
        }
    }
    return count;
}

// Entrada da tabela de encaminhamento
typedef struct {
    int *path;
} RoutingTableEntry;

// Tabela de encaminhamento
RoutingTable RoutuingTableEntry[MAX_NODES][MAX_NODES];

// Tabela de caminhos mais curtos
ShortestPathTable RoutingTableEntry[MAX_NODES];

// Tabela de expedição
ExpeditionTable int[MAX_NODES];

// inicializa a tabela de encaminhamento
void initializeRoutingTable() {
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            strcpy(RoutingTable[i][j], "");
        }
    }
}

// inicializa a tabela dos caminhos mais curtos
void initializeShortestPathTable() {
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            strcpy(ShortPathTable[i], "");
        }
    }
    ShortestPathTable[personal] = personal;
}

// inicializa a tabela de expedição
void initializeExpeditionTable() {
    for (int i = 0; i < MAX_NODES; i++) {
        ExpeditionTable[i] = -1;
    }
    
}

// receber informação dos vizinhos
void receiveRoutingInfo(int origin, int destination, int* pathArray) {
     
   for (int i = 1; pathArray[i] = destination, i++) {
        if (pathArray[i-1] == origin) {
            return;
        }
        else (updateTables(origin, destination, pathArray, personal));


   } 
}

// atualizar as tabelas
void updateTables(int origin, int destination, int *pathArray, int personal) {

   pathSize = sizeof(pathArray)/2;
   char* path = ArrayToString(pathArray, pathSize, personal)

   //Atualizar a tabela de encaminhamento
        strcpy(RoutingTable[origin][destination], path);
    //Atualizar a tabela de caminhos mais curtos
    int minHyphens = -1;
    char *minPath = NULL;
    int hyphens = 0;
    for (int i = 0; i < size; i++) {
         hyphens = countHyphens(RoutingTable[i]);

        if (minHyphens == -1 || hyphens < minHyphens) {
            minHyphens = hyphens;
        }
        if(RoutingTable[i] != "-")  {
            minPath = RoutingTable[i];
        }
        
    }
    
    if(minPath != ShortPatrhTable[destination]){
        strcpy(ShortPathTable[destination], minPath);
    }

    // Atualizar a tabela de expedição
    int expPath = 0;
    int count = 0;
    char secondNum[3] = {0}; 

    // Encontrar e extrair o segundo número da string minPath
    for (int i = 0; minPath[i] != '\0'; i++) {
        if (minPath[i] == '-') {
            count++;
            if (count == 1) { // Se é o primerio "-", o proximo numero é o segundo numero
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

  
void printRoutingTable(int personal) {
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            if (strlen(RoutingTable[i][j]) == 0) {
                printf("Route from %d to %d by %d: -\n", personal, j, i);
            } else {
                printf("Route from %d to %d by %d: %s\n", personal, j, i, RoutingTable[i][j]);
            }
        }
    }
}

void printShortestPathTable() {
    for (int i = 0; i < MAX_NODES; i++) {
        
            printf("Shortest path to %d: %s\n", i, ShortPathTable[i]);
        }
}


void printExpeditionTable() {
    for (int i = 0; i < MAX_NODES; i++) {
        if (ExpeditionTable[i] == -1) {
            printf("Expedition to %d: -\n", i);
        } else {
            printf("Expedition to %d: %d\n", i, ExpeditionTable[i]);
        }
    }
}





// // Enviar informações de encaminhamento para os vizinhos
void sendRoutingInfo(int origin, int destination, int* pathArray) {}





