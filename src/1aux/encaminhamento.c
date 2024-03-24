#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <main.h>

#define MAX_NODES 100
#define MAX_Arr_LENGTH 256

char Arr[MAX_Arr_LENGTH];

char* ArrayToString(int Arr[], int size) {
    // Calculate the length of the resulting string
    int total_length = size * 3; // 2 digits + 1 '-' separator for each element, plus 1 for the null terminator
    char* result = malloc(total_length * sizeof(char));
    if (result == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    // Initialize the result string
    result[0] = '\0';

    // Iterate over the Array and append each element to the result string
    for (int i = 0; i < size; i++) {
        // Convert the integer to a string
        char num_str[3]; // Assuming each number has at most 2 digits
        snprintf(num_str, sizeof(num_str), "%02d", Arr[i]); // Ensures that each number is represented with at least 2 digits

        // Append the string representation of the number to the result string
        strcat(result, num_str);

        // Append a '-' separator unless it's the last element
        if (i < size - 1) {
            strcat(result, "-");
        }
    }

    return result;
}


// Tabela de encaminhamento
RoutingTableEntry path[MAX_NODES][MAX_NODES];

// inicializa a tabela de encaminhamento
void initializeRoutingTable() {
    for (int i = 0; i < MAX_NODES; i++) {
        for (int j = 0; j < MAX_NODES; j++) {
            strcpy(routingTable[i][j], "");
        }
    }
}

// receber informação dos vizinhos
void receiveRoutingInfo(int origin, int destination, int* pathArray) {
     
   for (int i = 1; pathArray[i] = destination, i++) {
        if (pathArray[i-1] == origin) {
            return;
        }
        else (updateRoutingTable(origin, destination, pathArray));

   } 
// Quando recebemos 'ROUTE origin destination Arr <LF>' de um nó vizinho, atualizamos a tabela de encaminhamento
void updateRoutingTable(int origin, int destination, int *pathArray) {

   pathSize = sizeof(pathArray)/2;
   char* path = ArrayToString(pathArray, pathSize)
   
    strcpy(routingTable[origin][destination], path);
}

// Enviar informações de encaminhamento para os vizinhos
void sendRoutingInfo(int origin, int destination, int* pathArray) {



}

