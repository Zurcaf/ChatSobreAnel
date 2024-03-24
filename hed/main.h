#ifndef MAIN_H
#define MAIN_H

//includes de bibliotecas necessárias
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <signal.h>

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
#define MAX_NODES 16
#define MAX_IDS 100
#define MAX_BUFFER 200
#define MAX_RESPONSE 1000
#define INIT_IP "000000000000000"
#define WAIT_TIME 10

#define MAX_PUT 100
#define WORD_LEN_MAX 70

typedef struct _tcpServerInfo tcpServerInfo;
typedef struct _tcpServerInfo
{
    int id;
    char IP[MAX_IP_LENGTH];
    int TCP;
    int fd;
} tcpServerInfo;

typedef struct _nodeClinetInfo tcpClientInfo;
typedef struct _nodeClinetInfo
{
    int id;
    int fd;
    tcpClientInfo *next;
} tcpClientInfo;

typedef struct _udpServer udpServer;
typedef struct _udpServer
{
    char regIP[MAX_IP_LENGTH];
    int regUDP;
    int fd;
} udpServer;


#endif