#include "../hed/main.h"
#include "../hed/auxFunc.h"

int inputCheck(char* input, int *inputCount, char** inputArray)
{
    *inputCount = 0;
    char *token;

    token = strtok(input, " ");

    while (*inputCount < MAX_ARGUMENTS)
    {
        if (token == NULL)
            break;

        inputArray[*inputCount] = (char *)malloc(strlen(token) + 1);
        memoryCheck(inputArray[*inputCount]);

        strcpy(inputArray[*inputCount], token);
        strcat(inputArray[*inputCount], "\0");

        token = strtok(NULL, " ");
        *inputCount+= 1;
    }
    
    inputArray[*inputCount - 1][strlen(inputArray[*inputCount - 1]) - 1] = '\0'; // Remover o \n do último argumento
    inputArray[*inputCount] = NULL;                                  // Marcar o final da lista de argumentos

    if (strcmp(inputArray[0], "join") == 0 || strcmp(inputArray[0], "j") == 0)
    {
        // verificação do numero de argumentos para o join
        if (*inputCount != 3)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for join command!\n");
            return (0);
        }

        // verificação do formato dos argumentos para o join
        if (strlen(inputArray[1]) == 3 && strlen(inputArray[2]) == 2)
        {
            if ((isdigit(inputArray[1][0]) && isdigit(inputArray[1][1]) && isdigit(inputArray[1][2])))
            {
                if (isdigit(inputArray[2][0]) && isdigit(inputArray[2][1]))
                    return (1);
            }
        }
        
        fprintf(stderr, "ERROR: wrong format of the join arguments!\n");
        fprintf(stderr, "\tring formate: 000\n");
        fprintf(stderr, "\tid formate: 00\n");
        return (0);
    }

    if (((strcmp(inputArray[0], "direct") == 0) && strcmp(inputArray[1], "join") == 0) || strcmp(inputArray[0], "dj") == 0)
    {
        if (*inputCount != 5 && *inputCount != 6)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for direct join command!\n");
            return (0);
        }
        if (strlen(inputArray[2]) == 2 && strlen(inputArray[3]) == 2 && strlen(inputArray[4]) == MAX_IP_LENGTH && strlen(inputArray[5]) == 5)
        {
            if (isdigit(inputArray[2][0]) && isdigit(inputArray[2][1]))
            {
                if (isdigit(inputArray[3][0]) && isdigit(inputArray[3][1]))
                {
                    if (ValidIPAddress(inputArray[4]) == false)
                    {
                        if (atoi(inputArray[5]) > 0 && atoi(inputArray[5]) < 65536)
                            return (2);
                    }
                }
            }
        }
        fprintf(stderr, "ERROR: wrong format of the direct join arguments!\n");
        fprintf(stderr, "\tid formate: 00\n");
        fprintf(stderr, "\tSucc id formate: 00\n");
        fprintf(stderr, "\tSucc IP formate: 000.000.000.000\n");
        fprintf(stderr, "\tSucc Port formate: 00000\n");

        return 0;

    }
    
    if (strcmp(inputArray[0], "chord") == 0 || strcmp(inputArray[0], "c") == 0)
    {
        if (*inputCount != 1)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for chord command!\n");
            return (0);
        }
        return (3);
    }

    if (((strcmp(inputArray[0], "remove") == 0) && strcmp(inputArray[1], "chord") == 0) || strcmp(inputArray[0], "rc") == 0)
    {
        if (*inputCount != 1)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for remove command!\n");
            return (0);
        }
        return (4);
    }

    if (((strcmp(inputArray[0], "show") == 0) && strcmp(inputArray[1], "topology") == 0) || strcmp(inputArray[0], "st") == 0)
    {
        if (*inputCount != 1)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for remove command!\n");
            return (0);
        }
        return (5);
    }

    if (((strcmp(inputArray[0], "show") == 0) && strcmp(inputArray[1], "routing") == 0) || strcmp(inputArray[0], "sr") == 0)
    {
        if (*inputCount != 2)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for remove command!\n");
            return (0);
        }
        return (6);
    }

    if(((strcmp(inputArray[0], "show") == 0) && strcmp(inputArray[1], "path") == 0) || strcmp(inputArray[0], "sp") == 0)
    {
        if (*inputCount != 2)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for remove command!\n");
            return (0);
        }
        return (7);
    }

    if (((strcmp(inputArray[0], "show") == 0) && strcmp(inputArray[1], "forwarding") == 0) || strcmp(inputArray[0], "sf") == 0)
    {
        if (*inputCount != 1)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for remove command!\n");
            return (0);
        }
        return (8);
    }

    if(strcmp(inputArray[0], "message") == 0 || strcmp(inputArray[0], "m") == 0)
    {
        if (*inputCount != 3)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for message command!\n");
            return (0);
        }
        return (9);
    }

    if(strcmp(inputArray[0], "leave") == 0 || strcmp(inputArray[0], "l") == 0)
    {
        if (*inputCount != 1)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for leave command!\n");
            return (0);
        }
        return (10);
    }

    if (strcmp(inputArray[0], "exit") == 0 || strcmp(inputArray[0], "x") == 0)
    {
        printf("Exiting the application...\n");
        return (11);
    }
    
    fprintf(stderr, "ERROR: command not found!\n\n");
    
    return 0;
}

int newID(char* buffer, NodeInfo *personal, NodeInfo *succ, NodeInfo *succ2, NodeInfo *pred)
{
    int lineCount = 0;
    char **lines = (char **)malloc(MAX_NODES * sizeof(char *));
    memoryCheck(lines);
    char *token;

    token = strtok(buffer, "\n");

    while (lineCount < 17)
    {
        if (token == NULL)
            break;

        lines[lineCount] = (char *)malloc(strlen(token) + 1);
        memoryCheck(lines[lineCount]);
        

        strcpy(lines[lineCount], token);
        strcat(lines[lineCount], "\0");

        token = strtok(NULL, "\n");
        lineCount += 1;
    }
    
    //alocar memoria para um vetor de nós
    NodeInfo **nodes = (NodeInfo **)malloc(MAX_NODES * sizeof(NodeInfo*));
    memoryCheck(nodes);

    for (int i = 0; i < MAX_NODES; i++)
    {
        //alocar memória para os nós
        nodes[i] = (NodeInfo *)malloc(sizeof(NodeInfo));
        memoryCheck(nodes[i]);
        
        //inicilizar os nós
        nodes[i]->id = -1;
        strcpy(nodes[i]->IP, INIT_IP);
        nodes[i]->TCP = -1;
    }
    
    int aux1, aux2;
    char aux3[MAX_IP_LENGTH];

    //retirar informação sobre os nós
    for (int i = 1; i < lineCount; i++)
    {
        sscanf(lines[i], "%d %s %d", &aux1, aux3, &aux2);
        nodes[aux1]->id = aux1;
        strcpy(nodes[aux1]->IP, aux3);
        nodes[aux1]->TCP = aux2;
    }

    //libertar memoria das linhas
    for (int i = 0; i < 17; i++)
        free(lines[i]);
    
    free(lines);
    
    if (nodes[personal->id]->id != -1)
    {
        for (int i = personal->id; i < MAX_NODES; i++)
        {
            if (nodes[i]->id == -1)
            {
                personal->id = i;
                break;
            }
        }
        printf("ID already in use. New ID: %02d\n", personal->id);
    }
    if (nodes[personal->id]->id != -1)
    {
        for (int i = 0; i < personal->id; i++)
        {
            if (nodes[i]->id == -1)
            {
                personal->id = i;
                break;
            }
        }
        printf("ID already in use. New ID: %02d\n", personal->id);
    }

    //inicilizar succ, succ2 e pred ids
    succ->id = personal->id;
    succ2->id = personal->id;
    pred->id = personal->id;
        
    //encontrar o sucessor
    if (nodes[succ->id]->id == -1 && lineCount > 0)
    {
        for (int i = personal->id + 1; i < MAX_NODES; i++)
        {
            if (nodes[i]->id != -1)
            {
                succ->id = i;
                break;
            }
        }
    }
    if (nodes[succ->id]->id == -1 && lineCount != 0)
    {    
        for (int i = 0; i < personal->id; i++)
        {
            if (nodes[i]->id != -1)
            {
                succ->id = i;
                break;
            }
        }
    }

    strcpy(succ->IP, nodes[succ->id]->IP);
    succ->TCP = nodes[succ->id]->TCP;

    for (int i = 0; i < MAX_NODES; i++)
        free(nodes[i]);

    free(nodes);

    return lineCount;
}


//Implementação dos comandos da interface com o utlizador (4.2)
bool join(NodeInfo *personal, NodeInfo *succ, NodeInfo *succ2, NodeInfo *pred, ServerInfo server, int ring)
{
    int fd, errcode;
    char buffer[200];

    char aux_str[8];

    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;
    struct addrinfo hints, *res;

    //inicializar buffer
    for(int i = 0; i < 200; i++)
        buffer[i] = '0';

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
        printf("Error connecting");
        exit(1);
    }

    sprintf(buffer, "NODES %03d", ring);
    n = sendto(fd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    { /*error*/
        printf("Error messaging.");
        exit(1);
    }

    for(int i = 0; i < 200; i++)
        buffer[i] = '0';

    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 200, 0, &addr, &addrlen);
    if (n == -1) /*error*/
        exit(1);

    buffer[n] = '\0';

    // Confirmar que o ID do nó não está a ser usado
    newID(buffer, personal, succ, succ2, pred);

    // Registo do nó no servidor
    for(int i = 0; i < 200; i++)
        buffer[i] = '0';

    sprintf(buffer, "REG %03d %02d %s %05d", ring, personal->id, personal->IP, personal->TCP);
    n = sendto(fd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    { /*error*/
        printf("Error messaging.");
        exit(1);
    }

    for(int i = 0; i < 200; i++)
        buffer[i] = '0';

    freeaddrinfo(res);

    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 200, 0, &addr, &addrlen);
    if (n == -1) /*error*/
        exit(1);

    buffer[n] = '\0';

    printf("%s\n", buffer);

    close(fd);
    return true;
}

bool directJoin(NodeInfo personal, NodeInfo Succ)
{
    

    return true;
}

void leave(int ring, NodeInfo personal, ServerInfo server)
{
    int fd, errcode;
    char aux_str[8];
    char buffer[200];

    //inicializar buffer
    for(int i = 0; i < 200; i++)
    {
        buffer[i] = '0';
    }

    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;
    struct addrinfo hints, *res;

    // socket creation and verification
    fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    if (fd == -1)                        /*error*/
        exit(1);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket

    sprintf(aux_str, "%d", server.regUDP);

    errcode = getaddrinfo(server.regIP , aux_str, &hints, &res);
    if (errcode != 0)
    { /*error*/
        printf("Error connecting");
        exit(1);
    }

    sprintf(buffer, "UNREG %03d %02d", ring, personal.id);
    n = sendto(fd, buffer, strlen(buffer), 0, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    { /*error*/
        printf("Error messaging.");
        exit(1);
    }

    for(int i = 0; i < 200; i++)
        buffer[i] = '0';

    freeaddrinfo(res);

    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 200, 0, &addr, &addrlen);
    if (n == -1) /*error*/
        exit(1);

    buffer[n] = '\0';

    printf("%s\n", buffer);

    close(fd);
    return;
}

void ShowTopology(NodeInfo personal, NodeInfo succ, NodeInfo succ2, NodeInfo pred)
{
    printf("Me: \n ID: %02d\n, IP: %s\n, TCP: %d\n",  personal.id, personal.IP, personal.TCP);
    printf("Succ: \n ID: %02d\n, IP: %s\n, TCP: %d\n",  succ.id, succ.IP, succ.TCP);
    printf("Succ2: \n ID: %02d\n, IP: %s\n, TCP: %d\n",  succ2.id, succ2.IP, succ2.TCP);
    printf("completar com a informação da corda, se existir...");
    return;

}