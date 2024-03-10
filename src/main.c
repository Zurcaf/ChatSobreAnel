// Invocação da aplicação ChatOnRing
//  Aqui localizam-se o main do programa, onde são invocadas as funções da aplicação ChatOnRing.
#include "../hed/main.h"
#include "../hed/auxFunc.h"
#include "../hed/interface.h"

int main(int argc, char *argv[])
{
    char IP[MAX_IP_LENGTH] = "";
    int TCP = 0;

    char regIP[MAX_IP_LENGTH] = "193.136.138.142";
    int regUDP = 59000;

    int argumentCount = 0;
    char **arguments = NULL;
    
    char input[MAX_PUT];


    NodeInfo *personal = (NodeInfo *)malloc(sizeof(NodeInfo));
    memoryCheck(personal);

    // Verifing the arguments passed to the application
    argsCheck(argc, argv, IP, &TCP, regIP, &regUDP);

    strcpy(personal->IP, IP);
    personal->TCP = TCP;

    if (argc == 5)
    {
    strcpy(personal->regIP, regIP);
    personal->regUDP = regUDP;
    }

    printf("------------------------------------------------------------\n");
    printf("Application COR invoked with the following parameters:\n");
    printf("IP: %s\n", IP);
    printf("TCP: %d\n", TCP);
    printf("regIP: %s\n", regIP);
    printf("regUDP: %d\n", regUDP);
    printf("------------------------------------------------------------\n");
    printf("Available comands:\njoin (j) ring id\ndirect join (dj) id succid succIP succTCP\nchord (c)\nremove chord (rc)\nshow topology (st)\nshow routing (sr) dest\nshow path (sp) dest\nshow forwarding (sf)\nmessage (m) dest message\nleave (l)\nexit (x)\n");
    printf("------------------------------------------------------------\n");

    while (1)
    {
        printf("Insert command: ");
        fgets(input, sizeof(input), stdin);

        switch (inputCheck(input, &argumentCount, arguments))
        {
            case 0:
                break;

            case 1:
                join(IP, TCP, regIP, regUDP, arguments[1], arguments[2]);
                continue;

        }

        

        for (int i = 0; arguments[i] != NULL; ++i)
        {
            free(arguments[i]);
        }
        free(arguments);
    }

    free(personal);

    return 0;
}


