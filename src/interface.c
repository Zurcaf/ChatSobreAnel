#include "../hed/main.h"
#include "../hed/auxFunc.h"

int inputCheck(char* input, int *inputCount, char** inputArray)
{
    int i = 0;
    *inputCount = 0;
    char *token;


    token = strtok(input, " ");

    if (token == NULL)
    {
        fprintf(stderr, "ERROR: wrong number of arguments!\n");
        return (0);
    }

    inputArray = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
    memoryCheck(inputArray);


    while (*inputCount < MAX_ARGUMENTS)
    {
        if (token == NULL)
            break;
        
        inputArray[*inputCount] = (char *)malloc(strlen(token) + 1);
        memoryCheck(inputArray[*inputCount]);

        strcpy(inputArray[*inputCount], token);
        strcat(inputArray[*inputCount], "\0");

        token = strtok(NULL, " ");
        *inputCount++;
    }

    inputArray[i - 1][strlen(inputArray[i - 1]) - 1] = '\0'; // Remover o \n do último argumento
    inputArray[i] = NULL;                                    // Marcar o final da lista de argumentos

    if (strcmp(inputArray[0], "join") == 0 || strcmp(inputArray[0], "j") == 0)
    {
        // verificação do numero de argumentos para o join
        if (*inputCount != 3)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for join command!\n");
            return (0);
        }


        if (strlen(inputArray[1]) != 3 || strlen(inputArray[2]) != 2)
        {
            fprintf(stderr, "ERROR: wrong format of the join arguments!\n");
            return (0);
        }
     
        if ((isdigit(inputArray[1][0]) && isdigit(inputArray[1][1]) && isdigit(inputArray[1][2])))
        {
            if (isdigit(inputArray[2][0]) && isdigit(inputArray[2][1]))
                return (1);
        }
        fprintf(stderr, "ERROR: wrong format of the join arguments!\n");
        fprintf(stderr, "\tring formate: 000\n");
        fprintf(stderr, "\tid formate: 00\n");
        return (0);
    }
    
    if (strcmp(inputArray[0], "exit") == 0 || strcmp(inputArray[0], "x") == 0)
    {
        printf("Exiting the application...\n");
        return (11);
    }
    
    return (0);
    fprintf(stderr, "ERROR: command not found!\n");
    
}

//Implementação dos comandos da interface com o utlizador (4.2)
bool join(char *IP, int TCP, char *regIP, int regUDP, char *ring, char *id)
{
    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;
    char buffer[128];

    struct addrinfo hints, *res;
    int fd, errcode, nchars;
    
    //socket creation and verification
    fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    if (fd == -1) /*error*/
        exit(1);

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket
    
    errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", "59000", &hints, &res);
    // errcode = getaddrinfo(regIP, regUDP, &hints, &res);
    if (errcode != 0)
    { /*error*/
        printf("Error connecting");
        exit(1);
    }

    //inicializar buffer
    for (int i = 0; i< strlen(buffer); i++)
    {
        buffer[i] = '\0';
    }

    
    sprintf(buffer, "NODES %s", ring);
    nchars = strlen(buffer);
    n = sendto(fd, buffer, nchars, 0, res->ai_addr, res->ai_addrlen);

    //inicializar buffer
    for (int i = 0; i< strlen(buffer); i++)
    {
        buffer[i] = '\0';
    }

    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 500, 0, &addr, &addrlen);
    if (n == -1) /*error*/
        exit(1);
    
    buffer[n] = '\0';
    printf("echo: %s\n", buffer);

    for (int i = 0; i< strlen(buffer); i++)
    {
        buffer[i] = '\0';
    }

    // = sendto(fd, "UNREG 112 12 ", 32, 0, res->ai_addr, res->ai_addrlen);  // UNREG 112 05   ///NODES 112
    sprintf(buffer, "REG %s %s %s %d", ring, id, IP, TCP);
    nchars = strlen(buffer);
    n = sendto(fd, buffer, nchars, 0, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    { /*error*/
        printf("Error messaging.");
        exit(1);
    }

    freeaddrinfo(res);

    for (int i = 0; i< strlen(buffer); i++)
    {
        buffer[i] = '\0';
    }
        
    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 500, 0, &addr, &addrlen);

    if (n == -1) /*error*/
        exit(1);

    buffer[n] = '\0';

    printf("echo: %s\n", buffer);
    close(fd);
    
    return true;
}