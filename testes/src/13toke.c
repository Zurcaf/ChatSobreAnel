#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int main ()
{
    char* buffer;

    buffer = (char *)malloc(128 * sizeof(char));
    strcpy(buffer, "NODESLIST 067\n01 142.0.0.1 1000\n02 140.0.0.1 2000\n03 140.0.0.1 2000\n04 140.0.0.1 2000\n\0");

    printf("%s\n", buffer);

    int inputCount = 0;
    char **inputArray = (char **)malloc(128 * sizeof(char *));
    char *token;

    token = strtok(buffer, "\n");

    while (inputCount < 17)
    {
        if (token == NULL)
            break;

        inputArray[inputCount] = (char *)malloc(strlen(token) + 1);
        if (inputArray[inputCount] == NULL)
        {
            fprintf(stderr, "ERROR: memory allocation failed!\n");
            exit(1);
        }
        

        strcpy(inputArray[inputCount], token);
        strcat(inputArray[inputCount], "\0");

        token = strtok(NULL, "\n");
        inputCount += 1;
    }
    

    char* id = (char *)malloc(3 * sizeof(char));
    char* ip = (char *)malloc(15 * sizeof(char));
    int port = 0;

    for (int i = 1; i < inputCount; i++)
    {
        printf("%s\n", inputArray[i]);
        sscanf(inputArray[i], "%s %s %d", id, ip, &port);
        printf("ID: %s\n", id);
        printf("IP: %s\n", ip);
        printf("PORT: %d\n\n", port);
    }
    
    

}