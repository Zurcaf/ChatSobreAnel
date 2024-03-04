#ifndef MAIN_H
#define MAIN_H

//includes de bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>


//definições de constantes
#define MAX_IP_LENGTH 15 // IPv4 has at most 15 characters

#define WORD_LEN_MAX 70
#define LINE_MAX 1000

typedef struct _edge Edge;

typedef struct _edge
{
    int n2;
    int wt;
    Edge *next;
} Edge;


typedef struct graph
{
    int vertices, maxMode;
    Edge **adj; 
}graph;

//include de outros ficheiros .h 
//#include "1Pals.h"

//verificações dos argumentos (quantidade e extensões)
void argsCheck(int argc, char *argv[], char *IP, int *TCP, char *regIP, int *regUDP);

//verificação de endereço IP
bool ValidIPAddress(const char *ipAddress);

//verificação de memoria alocada no endereço
void memoryCheck(void *ptr);

//verificar se o ficheiro foi bem aberto
void fileCheck(FILE *filePointer, char *fileName);

//libertação de toda a memoria alocada
void freeMemory(int maxLen, graph **gs, int *palsOrder, int *dictLenCount, char *statsLocation, char *dictLocation, char *palsLocation);

#endif