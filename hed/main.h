#ifndef MAIN_H
#define MAIN_H

//includes de bibliotecas necessárias
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include <unistd.h>

//definições de constantes
#define MAX_IP_LENGTH 16 // IPv4 has at most 15 characters +1 for the null terminator
#define MAX_PORT_LENGTH 5 // 65535 has 5 characters +1 for the null terminator
#define MAX_ARGUMENTS 7
#define INIT_ID "00"
#define INIT_IP "0000000000000000"

#define MAX_PUT 100
#define WORD_LEN_MAX 70

typedef struct _nodeInfo NodeInfo;

typedef struct _nodeInfo
{
    char id[2];
    char IP[MAX_IP_LENGTH];
    int TCP;
} NodeInfo;

typedef struct _ring RingInfo;
typedef struct _ring
{
    
} RingInfo;

#endif