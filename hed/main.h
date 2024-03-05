#ifndef MAIN_H
#define MAIN_H

//includes de bibliotecas necessárias
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>

//definições de constantes
#define MAX_IP_LENGTH 16 // IPv4 has at most 15 characters +1 for the null terminator
#define MAX_ARGUMENTS 7

#define WORD_LEN_MAX 70
#define LINE_MAX 1000

typedef struct _nodeInfo NodeInfo;

typedef struct _nodeInfo
{
    char *id;
    char *IP;
    int *TCP;
} NodeInfo;


//include de outros ficheiros .h 
//#include "interface.h"

bool join(char *IP, int TCP, char *regIP, char *regUDP, char *ring, char *id);

bool leave(char *IP, int TCP, char *regIP, int regUDP, char *ring, char *id);

//verificações dos argumentos (quantidade e extensões)
void argsCheck(int argc, char *argv[], char *IP, int *TCP, char *regIP, int *regUDP);

//verificação de endereço IP
bool ValidIPAddress(const char *ipAddress);

//verificação de memoria alocada no endereço
void memoryCheck(void *ptr);

//verificar se o ficheiro foi bem aberto
void fileCheck(FILE *filePointer, char *fileName);



#endif