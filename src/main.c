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

    // ssize_t n, nw;
    // struct sockaddr addr;
    // socklen_t addrlen;

    // Variáveis para o buffer
    // char *ptr, buffer[128];

    // variaveis com informação do servidor e do nó   
    int ring = 0;
    ServerInfo server;
    NodeInfo personal;
    NodeInfo succ;
    NodeInfo succ2;
    NodeInfo pred;

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
        SETs_Init(&readfds, &maxfd, personal.fd, succ.fd, succ2.fd, pred.fd);

        printf("Insert command: \n");

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





//     

//     while (1)
//     {
//  
//         printf("Waiting for activity on socket or stdin...\n");

//         if (select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1)
//             exit(1); /* error */

//         if (FD_ISSET(fd, &readfds)) 
//         {
//             // Novas conexões no socket
//             addrlen = sizeof(addr);
//             printf("Waiting to accept connections \n");
//             if ((newfd = accept(fd, &addr, &addrlen)) == -1)
//                 exit(1); /* error */

//             printf("Connection accepted\n");
//             while ((n = read(newfd, buffer, 128)) != 0)
//             {
//                 if (n == -1)
//                     break;   // exit(1);

//                 ptr = &buffer[0];
//                 while (n > 0)
//                 {
//                     if ((nw = write(newfd, ptr, n)) <= 0)
//                         exit(1);
//                     n -= nw;
//                     ptr += nw;
//                     printf("%s", buffer);
//                 }
//                 for (int i = 0; i < 128; i++)
//                 {
//                     buffer[i] = '\0';
//                 }
//                 ptr = NULL;
//             }
//             close(newfd);
//         }

//         if (FD_ISSET(STDIN_FILENO, &readfds)) 
//         {
//             // Entrada padrão (stdin)
//             printf("Data available on stdin...\n");
//             // Faça algo com a entrada padrão
//             fgets(buffer, sizeof(buffer), stdin);
//             printf("You entered: %s", buffer);

//             if (strcmp(buffer, "x\n") == 0)
//                 break;
//         }
//     }

//     close(fd);

//     freeaddrinfo(res);
//     close(fd);

