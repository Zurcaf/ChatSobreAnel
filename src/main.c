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

    //inicializat personal
    personal->id = NULL;
    personal->IP = NULL;
    personal->TCP = 0;
    personal->regIP = NULL;
    personal->regUDP = 0;

    // Verifing the arguments passed to the application
    argsCheck(argc, argv, IP, &TCP, regIP, &regUDP);

    personal->IP = (char *)malloc(strlen(IP) + 1);
    memoryCheck(personal->IP);

    strcpy(personal->IP, IP);
    personal->TCP = TCP;

    if (argc == 5)
    {
    personal->regIP = (char *)malloc(strlen(regIP) + 1);
    memoryCheck(personal->regIP);

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

    int aux;

    while (1)
    {
        printf("Insert command: ");
        fgets(input, sizeof(input), stdin);

        aux = inputCheck(input, &argumentCount, arguments);
        
        switch (aux)
        {
            case 0:
                continue;
            case 1:
                //join(IP, TCP, regIP, regUDP, arguments[1], arguments[2]);
                continue;
            case 2:
                //directJoin();
                continue;
            case 3:
                //chord();
                continue;
            case 4:
                //removeChord();
                continue;
            case 5:
                //showTopology();
                continue;
            case 6:
                //showRouting();
                continue;
            case 7:
                //showPath();
                continue;
            case 8:
                //showForwarding();
                continue;
            case 9:
                //message();
                continue;
            case 10:
                leave();
                continue;
            case 11:
                exit(0);
                break;
        }

        for (int i = 0; arguments[i] != NULL; ++i)
        {
            free(arguments[i]);
        }
        free(arguments);
    }
    
    free(personal->IP);
    free(personal->regIP);
    free(personal);

    return 0;
}


