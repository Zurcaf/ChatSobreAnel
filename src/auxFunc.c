#include "../hed/main.h"

void memoryCheck(void *ptr)
{
    if (ptr == NULL)
    {
        fprintf(stderr, "ERROR: not enough memory available!\n");
        exit(4);
    }
}

void fileCheck(FILE *filePointer, char *fileName)
{
    if (filePointer == NULL)
    {
        fprintf(stderr, "ERROR: could not open file %s!\n", fileName);
        exit(3);
    }
}

void bufferInit(char *buffer)
{
    for (int i = 0; i < MAX_BUFFER; i++)
        buffer[i] = '0';
    
    buffer[MAX_BUFFER] = '\0';
}

void inicializer(udpServer *server, tcpServerInfo *personal, tcpServerInfo *succ, tcpServerInfo *succ2, tcpClientInfo *pred)
{
    //inicializar server
    if (server != NULL)
    {
        strcpy(server->regIP, "193.136.138.142");
        server->regUDP = 59000;
    }
    
    if (personal != NULL)
    {
        //inicializar personal
        personal->id = -1;
        strcpy(personal->IP, INIT_IP);
        personal->TCP = -1;
        personal->fd = -1;
    }
    

    //inicializar succ
    succ->id = -1;
    strcpy(succ->IP, INIT_IP);
    succ->TCP = -1;
    succ->fd = -1;

    //inicializar succ2
    succ2->id = -1;
    strcpy(succ2->IP, INIT_IP);
    succ2->TCP = -1;
    succ2->fd = -1;

    //inicializar pred  
    pred->id = -1;
    pred->fd = -1;

}

void tcpServerInit(tcpServerInfo *server)
{
    struct addrinfo hints, *res;
    int errcode;
    char aux_str[8];    

    // Criação do socket
    if ((server->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit(1); // error
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE;

    //conversão de int para string
    sprintf(aux_str, "%d", server->TCP);
    
    if ((errcode = getaddrinfo(server->IP, aux_str, &hints, &res)) != 0)
        exit(1); /* error */

    if (bind(server->fd, res->ai_addr, res->ai_addrlen) == -1)
    {
        printf("error binding");
        exit(1);
    }

    if (listen(server->fd, 5) == -1)
        exit(1); /* error */

    freeaddrinfo(res);
}

void tcpClientInit(tcpServerInfo *tcpServer)
{
    struct addrinfo hints, *res;
    int errcode;
    char aux_str[8];

    // Criação do socket
    if ((tcpServer->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit(1); // error

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    //conversão de int para string
    sprintf(aux_str, "%d", tcpServer->TCP);

    if ((errcode = getaddrinfo(tcpServer->IP, aux_str, &hints, &res)) != 0)
        exit(1); /* error */

    if (connect(tcpServer->fd, res->ai_addr, res->ai_addrlen) == -1)
        exit(1); /* error */

    freeaddrinfo(res);
}

void tcpSend(int fdDest, char *buffer)
{
    ssize_t nbytes, nleft, nwritten;
    char *ptr;

    ptr = buffer;
    nbytes = strlen(buffer);
    nleft = nbytes;

    while (nleft > 0)
    {
        nwritten = write(fdDest, ptr, nleft);
        if (nwritten <= 0) /*error*/
            exit(1);
        nleft -= nwritten;
        ptr += nwritten;
    }
}

int tcpReceive (int fdRec, char *message)
{
    char *ptr;
    ssize_t n;

    ptr = message;
    while (1)
    {
        n = read(fdRec, ptr, 1);
        if (n < 0) /*error*/
            exit(1);
        if (n == 0)
        {
            //fim de transmissão
            return 0;
        }
        
        if (*ptr == '\n')
            break;
        ptr += n;
    }
    return 1;
}

void nodeServSend (udpServer server, char* buffer)
{
    char aux_str[8];
    int fd, errcode;

    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;
    struct addrinfo hints, *res;

    addrlen = sizeof(addr);


    // socket creation and verification
    fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    if (fd == -1)                        /*error*/
        exit(1);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket

    //conversão de int para string
    sprintf(aux_str, "%d", server.regUDP);

    //receber informação do servidor
    errcode = getaddrinfo(server.regIP, aux_str, &hints, &res);
    if (errcode != 0)
    { /*error*/
        printf("Error connectingsws");
        exit(1);
    }

    n = sendto(fd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    { /*error*/
        printf("Error messaging.");
        exit(1);
    }
    
    bufferInit(buffer);

    n = recvfrom(fd, buffer, strlen(buffer), 0, &addr, &addrlen);
    if (n == -1) /*error*/
        exit(1);

    buffer[n] = '\0';

    freeaddrinfo(res);
    close(fd);
}

void messageTokenize(char *message, char **inputArray, int *inputCount, char delim)
{
    char *token, buffer[MAX_BUFFER];

    strcpy(buffer, message);

    token = strtok(buffer, &delim);

    while (*inputCount < MAX_NODES)
    {
        if (token == NULL)
            break;

        inputArray[*inputCount] = (char *)malloc(strlen(token) + 1);
        memoryCheck(inputArray[*inputCount]);

        strcpy(inputArray[*inputCount], token);
        strcat(inputArray[strlen(token)], "\0");

        *inputCount+= 1;

        token = strtok(NULL, &delim);
    }
    inputArray[*inputCount] = NULL;

    int lastTokenLength = strlen(inputArray[*inputCount - 1]);

    for (int i=0; i<lastTokenLength; i++)
    {
        if (inputArray[*inputCount - 1][i] == '\n')
        {
            inputArray[*inputCount - 1][i] = '\0';
        }
    }
}

void SETs_Init(fd_set *readfds, int *maxfd, int personal_fd, int succ_fd, int succ2_fd, int pred_fd)
{
        FD_ZERO(readfds);  // Limpa o conjunto de descritores
        FD_SET(STDIN_FILENO, readfds);  // Adiciona o descritor de entrada padrão (stdin)

        *maxfd = STDIN_FILENO;

        FD_SET(personal_fd, readfds);  // Adiciona o descritor do socket
        *maxfd = (personal_fd > *maxfd) ? personal_fd : *maxfd;

        if (succ_fd != -1)
        {
            FD_SET(succ_fd, readfds);  // Adiciona o descritor do socket
            *maxfd = (succ_fd > *maxfd) ? succ_fd : *maxfd;
        }

        if (succ2_fd != -1)
        {
            FD_SET(succ2_fd, readfds);  // Adiciona o descritor do socket
            *maxfd = (succ2_fd >*maxfd) ? succ2_fd :*maxfd;
        }

        if (pred_fd != -1)
        {
            FD_SET(pred_fd, readfds);  // Adiciona o descritor do socket
           *maxfd = (pred_fd >*maxfd) ? pred_fd :*maxfd;
        }

}

bool ValidIPAddress(const char *ipAddress)
{
    int num, dots = 0;
    int last = -1;

    // Check if the IP length is reasonable
    if (strlen(ipAddress) > MAX_IP_LENGTH)
    {
        return true;
    }

    for (int i = 0; ipAddress[i] != '\0'; i++)
    {
        if (!isdigit(ipAddress[i]) && ipAddress[i] != '.')
        {
            return true;
        }

        if (ipAddress[i] == '.')
        {
            dots++;

            if (i == 0 || ipAddress[i + 1] == '\0')
            {
                return true;
            }

            num = atoi(ipAddress + last + 1);
            if (num < 0 || num > 255)
            {
                return true;
            }

            last = i;
        }

        if (dots > 3 || (i > 0 && ipAddress[i] == '.' && ipAddress[i - 1] == '.'))
        {
            return true;
        }

        if (i - last > 4)
        {
            return true;
        }

        if (isdigit(ipAddress[i]) && ipAddress[i + 1] == '\0')
        {
            num = atoi(ipAddress + last + 1);
            if (num < 0 || num > 255)
            {
                return true;
            }
        }
    }

    if (dots != 3)
    {
        return true;
    }

    return false;
}

void argsCheck(int argc, char *argv[], char *IP, int *TCP, char *regIP, int *regUDP)
{
    //Verificação do número de argumentos (2 ou 4) porque 
    //  regIP e regUDP são os parametros do servidor de registo e
    //  por omissão, estes parâmetros tomam os valores 193.136.138.142 e 59000, não sendo necessária a sua introdução.

    if (argc != 3 && argc != 5)
    {
        fprintf(stderr, "ERROR: wrong number of arguments!\n");
        exit(1);
    }
    
    //Verificação do IP
    if (ValidIPAddress(argv[1]))
    {
        fprintf(stderr, "ERROR: the entered IP address is not in a valid format!\n");
        exit(2);
    }

    strcpy(IP, argv[1]);
    
    //Verificação do TCP
    *TCP = atoi(argv[2]);
    if (*TCP < 1024 || *TCP > 65535)
    {
        fprintf(stderr, "ERROR: the entered TCP port is not in a valid format!\n");
        exit(2);
    }
    
    if (argc == 5)
    {
        //Verificação do regIP
        if (ValidIPAddress(argv[3]))
        {
            fprintf(stderr, "ERROR: the entered regIP address is not in a valid format!\n");
            exit(2);
        }

        //Verificação do regUDP
        *regUDP = atoi(argv[4]);
        if (*regUDP < 1024 || *regUDP > 65535)
        {
            fprintf(stderr, "ERROR: the entered regUDP port is not in a valid format!\n");
            exit(2);
        }        
    }
}

int getMessageType(char* message, char** messageArray)
{
    int inputCount = 0;
    messageTokenize(message, messageArray, &inputCount, ' ');

    if (strcmp(messageArray[0], "ENTRY") == 0)
    {
        return 1;
    }
    if (strcmp(messageArray[0], "PRED") == 0)
    {
        return 2;
    }
    if (strcmp(messageArray[0], "SUCC") == 0)
    {
        return 3;
    }
    if (strcmp(messageArray[0], "CHORD") == 0)
    {
        return 4;
    }
    if (strcmp(messageArray[0], "ROUTE") == 0)
    {
        return 5;
    }
    if (strcmp(messageArray[0], "CHAT") == 0)
    {
        return 6;
    }
    return 0;
}

