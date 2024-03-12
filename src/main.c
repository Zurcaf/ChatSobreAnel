// Invocação da aplicação ChatOnRing
//  Aqui localizam-se o main do programa, onde são invocadas as funções da aplicação ChatOnRing.
#include "../hed/main.h"
#include "../hed/auxFunc.h"
#include "../hed/interface.h"

int main(int argc, char *argv[])
{
    // Variáveis para os comandos do utilizador
    int argumentCount = 0, command = 0;
    char input[MAX_PUT];
    char **arguments = NULL;

    // Variáveis para o select
    fd_set readfds;
    int maxfd;
    int newfd;
    // int errcode;

    // variaveis com informação do servidor e do nó   
    int ring = 0;
    ServerInfo server;
    NodeInfo personal;
    NodeInfo succ;
    NodeInfo succ2;
    NodeInfo pred;

    // Inicialização das estruturas
    inicializer(&server, &personal, &succ, &succ2, &pred);

    // Alocação de memória para os argumentos
    arguments = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
    memoryCheck(arguments);

    // Verifing the arguments passed to the application
    argsCheck(argc, argv, personal.IP, &personal.TCP, server.regIP, &server.regUDP);

    // Inicialização do servidorTCP de escuta do nó
    TCPServerInit(&personal);

    // printf("------------------------------------------------------------\n");
    // printf("Application COR invoked with the following parameters:\n");
    // printf("IP: %s\n", personal.IP);
    // printf("TCP: %d\n", personal.TCP);
    // printf("regIP: %s\n", server.regIP);
    // printf("regUDP: %d\n", server.regUDP);
    printf("------------------------------------------------------------\n");
    printf("Available comands:\njoin (j) ring id\ndirect join (dj) id succid succIP succTCP\nchord (c)\nremove chord (rc)\nshow topology (st)\nshow routing (sr) dest\nshow path (sp) dest\nshow forwarding (sf)\nmessage (m) dest message\nleave (l)\nexit (x)\n");
    printf("------------------------------------------------------------\n");

    while (1)
    {
        // Inicialização dos descritores para o select
        SETs_Init(&readfds, &maxfd, personal.fd, succ.fd, succ2.fd, pred.fd);

        printf("Insert command: \n");

        // Espera por atividade com o select
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1)
            exit(1); /* error */

        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            fgets(input, sizeof(input), stdin);

            command = inputCheck(input, &argumentCount, arguments);

            switch (command)
            {
                case 0:
                    break;
                case 1:
                    ring = atoi(arguments[1]);
                    personal.id = atoi(arguments[2]);
                    join(personal, server, ring);
                    break;
                case 2:
                    personal.id = atoi(arguments[1]);
                    succ.id = atoi(arguments[2]);
                    strcpy(succ.IP, arguments[3]);
                    succ.TCP = atoi(arguments[4]);
                    directJoin(personal, succ);
                    break;
                case 3:
                    //chord();
                    break;
                case 4:
                    //removeChord();
                    break;
                case 5:
                    //showTopology();
                    break;
                case 6:
                    //showRouting();
                    break;
                case 7:
                    //showPath();
                    break;
                case 8:
                    //showForwarding();
                    break;
                case 9:
                    //message();
                    break;
                case 10:
                    leave(ring, personal, server);
                    break;
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

        if (FD_ISSET(personal.fd, &readfds))
        {
            // Novas conexões no socket
            if ((newfd = accept(personal.fd, NULL, NULL)) == -1)
                exit(1); /* error */

            printf("Connection accepted\n");
        }
    }

    close(personal.fd);
    return 0;
}


