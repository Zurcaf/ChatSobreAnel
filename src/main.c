// Invocação da aplicação ChatOnRing
//  Aqui localizam-se o main do programa, onde são invocadas as funções da aplicação ChatOnRing.

#include "../hed/main.h"
#include "../hed/interface.h"

// Invocação da aplicação pelo comando:
//  COR IP TCP regIP regUDP
//  IP é o endereço IP da máquina
//  TCP é o porto TCP onde a máquina vai escutar
int main(int argc, char *argv[])
{
    char IP[MAX_IP_LENGTH];
    int TCP = 0;

    char regIP[MAX_IP_LENGTH] = "193.136.138.142";
    int regUDP = 59000;

    int i = 0;
    char *token;
    char **arguments;
    int argCount = 0;
    bool flag_exit = true, flag_arg = true;

    // Verifing the arguments passed to the application
    argsCheck(argc, argv, IP, &TCP, regIP, &regUDP);

    char input[256]; // Maximum input length
    printf("------------------------------------------------------------\n");
    printf("Application COR invoked with the following parameters:\n");
    printf("IP: %s\n", IP);
    printf("TCP: %d\n", TCP);
    printf("regIP: %s\n", regIP);
    printf("regUDP: %d\n", regUDP);
    printf("------------------------------------------------------------\n");
    printf("Available comands:\njoin (j) ring id\ndirect join (dj) id succid succIP succTCP\nchord (c)\nremove chord (rc)\nshow topology (st)\nshow routing (sr) dest\nshow path (sp) dest\nshow forwarding (sf)\nmessage (m) dest message\nleave (l)\nexit (x)\n");
    printf("------------------------------------------------------------\n");

    while (flag_exit)
    {
        printf("Insert command: ");
        fgets(input, sizeof(input), stdin);

        token = strtok(input, " ");

        if (token == NULL)
        {
            fprintf(stderr, "ERROR: wrong number of arguments!\n");
            continue;
        }

        arguments = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
        memoryCheck(arguments);

        i = 0;
        argCount = 0;
        flag_arg = true;

        while (flag_arg && i < MAX_ARGUMENTS)
        {
            if (token == NULL)
            {
                break;
            }
            arguments[i] = (char *)malloc(strlen(token) + 1);
            memoryCheck(arguments[i]);

            strcpy(arguments[i], token);
            strcat(arguments[i], "\0");

            token = strtok(NULL, " ");
            argCount++;
            i++;
        }

        if (argCount != 0)
        {
            arguments[i - 1][strlen(arguments[i - 1]) - 1] = '\0'; // Remover o \n do último argumento
            arguments[i] = NULL;                                   // Marcar o final da lista de argumentos
        }

        if (strcmp(arguments[0], "join") == 0 || strcmp(arguments[0], "j") == 0)
        {
            // verificação do numero de argumentos para o join
            if (argCount != 3)
            {
                fprintf(stderr, "ERROR: wrong number of arguments for join command!\n");
                continue;
            }
            if (strlen(arguments[1]) != 3 || strlen(arguments[2]) != 2)
            {
                fprintf(stderr, "ERROR: wrong format of the join arguments!\n");
                continue;
                ;
            }
         
            if ((isdigit(arguments[1][0]) && isdigit(arguments[1][1]) && isdigit(arguments[1][2])))
            {
                if (isdigit(arguments[2][0]) && isdigit(arguments[2][1]))
                {
                    join(IP, TCP, regIP, regUDP, arguments[1], arguments[2]);
                }
                else
                {
                    fprintf(stderr, "ERROR: wrong format of the join arguments!\n");
                    fprintf(stderr, "\tring formate: 000\n");
                    fprintf(stderr, "\tid formate: 0aaaaa0\n");
                }
            }
            else
            {
                fprintf(stderr, "ERROR: wrong format of the join arguments!\n");
                fprintf(stderr, "\tring formate: 000\n");
                fprintf(stderr, "\tid formate: 00\n");
            }
        }
        else if (strcmp(arguments[0], "exit") == 0 || strcmp(arguments[0], "x") == 0)
        {
            printf("Exiting the application...\n");
            flag_exit = false;
        }
        else
        {
            fprintf(stderr, "ERROR: command not found!\n");
        }

        for (int i = 0; arguments[i] != NULL; ++i)
        {
            free(arguments[i]);
        }
        free(arguments);
    }

    return 0;
}
