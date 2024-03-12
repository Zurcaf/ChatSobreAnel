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

void inicializer(ServerInfo *server, NodeInfo *personal, NodeInfo *succ, NodeInfo *succ2, NodeInfo *pred)
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
        personal->id = 0;
        strcpy(personal->IP, INIT_IP);
        personal->TCP = 0;
        personal->fd = 0;
    }
    

    //inicializar succ
    succ->id = 0;
    strcpy(succ->IP, INIT_IP);
    succ->TCP = 0;
    succ->fd = 0;

    //inicializar succ2
    succ2->id = 0;
    strcpy(succ2->IP, INIT_IP);
    succ2->TCP = 0;
    succ2->fd = 0;

    //inicializar pred  
    pred->id = 0;
    strcpy(pred->IP, INIT_IP);
    pred->TCP = 0;
    pred->fd = 0;
}

void TCPServerInit(NodeInfo *server)
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

void TCPClientInit(NodeInfo *client)
{
    struct addrinfo hints, *res;
    int errcode;
    char aux_str[8];

    // Criação do socket
    if ((client->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit(1); // error

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    //conversão de int para string
    sprintf(aux_str, "%d", client->TCP);
    
    if ((errcode = getaddrinfo(client->IP, aux_str, &hints, &res)) != 0)
        exit(1);

    if (connect(client->fd, res->ai_addr, res->ai_addrlen) == -1)
        exit(1);

    freeaddrinfo(res);
}

void SETs_Init(fd_set *readfds, int *maxfd, int personal_fd, int succ_fd, int succ2_fd, int pred_fd)
{
        FD_ZERO(readfds);  // Limpa o conjunto de descritores
        FD_SET(STDIN_FILENO, readfds);  // Adiciona o descritor de entrada padrão (stdin)

        *maxfd = STDIN_FILENO;

        FD_SET(personal_fd, readfds);  // Adiciona o descritor do socket
        *maxfd = (personal_fd > *maxfd) ? personal_fd : *maxfd;

        if (succ_fd != 0)
        {
            FD_SET(succ_fd, readfds);  // Adiciona o descritor do socket
            *maxfd = (succ_fd > *maxfd) ? succ_fd : *maxfd;
        }

        if (succ2_fd != 0)
        {
            FD_SET(succ2_fd, readfds);  // Adiciona o descritor do socket
            *maxfd = (succ2_fd >*maxfd) ? succ2_fd :*maxfd;
        }

        if (pred_fd != 0)
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

