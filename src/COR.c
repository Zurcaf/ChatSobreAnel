//Invocação da aplicação ChatOnRing
// Aqui localizam-se o main do programa, onde são invocadas as funções da aplicação ChatOnRing.

#include "../hed/COR.h"

//Invocação da aplicação pelo comando:
// COR IP TCP regIP regUDP
// IP é o endereço IP da máquina
// TCP é o porto TCP onde a máquina vai escutar
int main(int argc, char* argv[])
{
char IP[MAX_IP_LENGTH];
int TCP = 0, i=0;

char regIP[MAX_IP_LENGTH] = "193.136.138.142";
int regUDP = 59000;

//Verifing the arguments passed to the application
argsCheck(argc, argv, IP, &TCP, regIP, &regUDP);

char input[256]; // Maximum input length
printf("------------------------------------------------------------\n");
printf("Application COR invoked with the following parameters:\n");
printf("IP: %s\n", IP);
printf("TCP: %d\n", TCP);
printf("regIP: %s\n", regIP);
printf("regUDP: %d\n", regUDP);
printf("------------------------------------------------------------\n\n");

printf("Available comands:\njoin (j) ring id\ndirect join (dj) id succid succIP succTCP\nchord (c)\nremove chord (rc)\nshow topology (st)\nshow routing (sr) dest\nshow path (sp) dest\nshow forwarding (sf)\nmessage (m) dest message\nleave (l)\nexit (x)\n");

while (flag_leave)
{
    scanf("%s", input);

    token = strtok(input, " ");

    if (token!=NULL)
    {
        printf("Command1: %s\n", token);
        strcpy(command, token);
        printf("Command2: %s\n", command);
        i = 0;

        while ((token = strtok(NULL, " ")) != NULL && i < MAX_ARGUMENTS) {
            arguments[i] = token;
            ++i;
        }
        arguments[i] = NULL; // Marcar o final da lista de argumentos

    }else
    {
        fprintf(stderr, "ERROR: wrong number of arguments!\n");
        continue;
    }
}

return 0;
}

