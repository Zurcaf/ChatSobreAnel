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
    
    if (strcmp(inputArray[0], "exit") == 0 || strcmp(inputArray[0], "x") == 0)
    {
        printf("Exiting the application...\n");
        return (11);
    }
    
    fprintf(stderr, "ERROR: command not found!\n\n");
    
    return 0;
}

//Implementação dos comandos da interface com o utlizador (4.2)
bool join(NodeInfo personal, ServerInfo server, int ring)
{
    int fd, errcode;
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

    errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", "59000", &hints, &res);
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
    {
        buffer[i] = '0';
    }

    freeaddrinfo(res);

    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 200, 0, &addr, &addrlen);
    if (n == -1) /*error*/
        exit(1);

    buffer[n] = '\0';

    newID(buffer, personal);

    int lineCount = 0;
    char **lines = (char **)malloc(128 * sizeof(char *));
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
    

    int id = 0;
    char* ip = (char *)malloc(15 * sizeof(char));
    int port = 0;
    bool flagID[99];

    //inicializar flagID
    for (int i = 0; i < 99; i++)
    {
        flagID[i] = false;
    }

    for (int i = 1; i < lineCount; i++)
    {
        sscanf(lines[i], "%d %s %d", &id, ip, &port);
        flagID[id] = true;
    }

    if (flagID[personal.id] == true)
    {        
        personal.id = 1;

        while (1)
        {
            if (flagID[personal.id]==false)
                break;
            personal.id++;
        }
        printf("New ID: %02d\n", personal.id);
    }

    

    close(fd);
    return true;
}

// char* IDCheck(char* id, )
// {
    
// }

bool leave()
{
    return true;
}