// Invocação da aplicação ChatOnRing
//  Aqui localizam-se o main do programa, onde são invocadas as funções da aplicação ChatOnRing.
#include "../hed/main.h"
#include "../hed/auxFunc.h"
#include "../hed/interface.h"

int main(int argc, char *argv[])
{
    int argumentCount = 0;
    char **arguments = NULL;
    
    int aux;
    char input[MAX_PUT];

    int ring = 0;
    ServerInfo server;
    NodeInfo personal;
    NodeInfo Succ;
    NodeInfo Succ2;
    NodeInfo Pred;

    //inicializar server
    strcpy(server.regIP, "193.136.138.142");
    server.regUDP = 59000;

    //inicializar personal
    personal.id = 0;
    strcpy(personal.IP, INIT_IP);
    personal.TCP = 0;

    //inicializar Succ
    Succ.id = 0;
    strcpy(Succ.IP, INIT_IP);
    Succ.TCP = 0;

    //inicializar Succ2
    Succ2.id = 0;
    strcpy(Succ2.IP, INIT_IP);
    Succ2.TCP = 0;

    //inicializar Pred  
    Pred.id = 0;
    strcpy(Pred.IP, INIT_IP);
    Pred.TCP = 0;

    // Alocação de memória para os argumentos
    arguments = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
    memoryCheck(arguments);

    
    // Verifing the arguments passed to the application
    argsCheck(argc, argv, personal.IP, &personal.TCP, server.regIP, &server.regUDP);

    printf("------------------------------------------------------------\n");
    printf("Application COR invoked with the following parameters:\n");
    printf("IP: %s\n", personal.IP);
    printf("TCP: %d\n", personal.TCP);
    printf("regIP: %s\n", server.regIP);
    printf("regUDP: %d\n", server.regUDP);
    printf("------------------------------------------------------------\n");
    printf("Available comands:\njoin (j) ring id\ndirect join (dj) id succid succIP succTCP\nchord (c)\nremove chord (rc)\nshow topology (st)\nshow routing (sr) dest\nshow path (sp) dest\nshow forwarding (sf)\nmessage (m) dest message\nleave (l)\nexit (x)\n");
    printf("------------------------------------------------------------\n");


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
                ring = atoi(arguments[1]);
                personal.id = atoi(arguments[2]);
                join(personal, server, ring);
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

    return 0;
}


