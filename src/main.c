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

    // Variável para a mensagem recebida
    int messageType, aux;
    char message[MAX_BUFFER];
    char **messageArray = NULL;

    // variaveis com informação do servidor e do nó   
    int ring = -1;
    ServerInfo server;
    NodeInfo personal;
    NodeInfo succ;
    NodeInfo succ2;
    NodeInfo pred;
    
    // ignorar o sinal SIGPIPE
    struct sigaction act;

    memset(&act,0,sizeof act);
    act.sa_handler=SIG_IGN;
    if(sigaction(SIGPIPE,&act,NULL)==-1)/*error*/exit(1);

    // Inicialização das estruturas
    inicializer(&server, &personal, &succ, &succ2, &pred);

    // Alocação de memória para os argumentos
    arguments = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
    memoryCheck(arguments);

    // Verifing the arguments passed to the application
    argsCheck(argc, argv, personal.IP, &personal.TCP, server.regIP, &server.regUDP);

    // Inicialização do servidorTCP de escuta do nó
    tcpServerInit(&personal);

    // printf("------------------------------------------------------------\n");
    // printf("Application COR invoked with the following parameters:\n");
    // printf("IP: %s\n", personal.IP);
    // printf("TCP: %d\n", personal.TCP);
    // printf("regIP: %s\n", server.regIP);
    // printf("regUDP: %d\n", server.regUDP);
    printf("------------------------------------------------------------\n");
    printf("Available comands:\njoin (j) ring id\ndirect join (dj) id succid succIP succTCP\nchord (c)\nremove chord (rc)\nshow topology (st)\nshow routing (sr) dest\nshow path (sp) dest\nshow forwarding (sf)\nmessage (m) dest message\nleave (l)\nexit (x)\n");
    printf("------------------------------------------------------------\n");
    printf("Insert command: \n");

    while (1)
    {
        // Inicialização dos descritores para o select
        SETs_Init(&readfds, &maxfd, personal.fd, succ.fd, succ2.fd, pred.fd);
        printf("maxfd: %d\n", maxfd);

        // Espera por atividade com o select
        printf ("Espera por atividade\n");
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }
        

        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            printf("stdin is set\n");
            fgets(input, sizeof(input), stdin);

            command = inputCheck(input, &argumentCount, arguments);

            switch (command)
            {
                case 0:
                    break;
                case 1:
                    ring = atoi(arguments[1]);
                    personal.id = atoi(arguments[2]);
                    join(&personal, &succ, &succ2, &pred, server, ring);
                    break;
                case 2:
                    personal.id = atoi(arguments[1]);
                    succ.id = atoi(arguments[2]);
                    strcpy(succ.IP, arguments[3]);
                    succ.TCP = atoi(arguments[4]);
                    directJoin(personal, &succ);
                    break;
                case 3:
                    //chord();
                    break;
                case 4:
                    //removeChord();
                    break;
                case 5:
                    showTopology(personal, succ, succ2, pred);
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
            command = 0;

            printf("Insert command: \n");
        }

        if (FD_ISSET(personal.fd, &readfds))
        {
            printf("canla de escuta is set\n");

            // Novas conexões no socket
            if ((newfd = accept(personal.fd, NULL, NULL)) == -1) 
                exit(1); /* error */
            
            tcpReceive(newfd, message);
            messageArray = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
            memoryCheck(messageArray);

            messageType = getMessageType(message, messageArray);

            printf("Canal Escuta: %s, %d\n", message, messageType);

            switch (messageType)
            {
                case 0:
                    printf("Error in the listening channel (comando não reconhecido)\n"); 
                    break;
                //caso seja um ENTRY
                case 1:
                    if (succ.id == personal.id)
                    {
                        pred.fd = newfd;
                        pred.id = atoi(messageArray[1]);
                        printf("registo de um PRED: id:%d, fd:%d\n", pred.id, pred.fd);

                        succ.id= atoi(messageArray[1]);
                        strcpy(succ.IP, messageArray[2]);
                        succ.TCP = atoi(messageArray[3]);

                        printf("registo de um PRED: id:%d, fd:%d\n", pred.id, pred.fd);
                        printf("registo de um SUCC: id:%d, IP:%s, TCP:%d\n", succ.id, succ.IP, succ.TCP);
                        tcpClientInit(&succ);
                        bufferInit(message);
                        
                        sprintf(message, "PRED %02d\n", personal.id);
                        tcpSend(succ.fd, message);
                    }
                    else
                    {
                        
                        tcpSend(pred.fd, message);
                        printf ("Envio de mensagem para o oldPred: %s",message);

                        pred.fd = newfd;
                        pred.id = atoi(messageArray[1]);

                        sprintf(message, "SUCC %02d %s %05d\n", succ.id, succ.IP, succ.TCP);
                        tcpSend(pred.fd, message);
                        printf ("Envio de mensagem para o predecessor: %s",message);
                    }
                    break;

                //caso seja um PRED
                case 2:
                    pred.fd = newfd;
                    pred.id = atoi(messageArray[1]);
                    printf("registo de um PRED: %d\n", pred.id);
                    break;

                //caso seja um CHORD
                case 4:
                    break;
                default:
                    break;
            }
            messageType = 0;

            for (int i = 0; messageArray[i] != NULL; ++i)
            {
                free(messageArray[i]);
            }
            free(messageArray);
        }

        if (FD_ISSET(succ.fd, &readfds))
        {
            printf("SUCC is set\n");

            aux = tcpReceive(succ.fd, message);

            printf("Mensagem recebida: %s, %d\n", message, aux);

            messageArray = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
            memoryCheck(messageArray);

            messageType = getMessageType(message, messageArray);

            printf ("Canal Sucessor: %s, %d\n", message, messageType);
        }
    }

    if(succ.fd != -1)
    {
        close(succ.fd);
    }
    if(succ2.fd != -1)
    {
        close(succ2.fd);
    }
    if(pred.fd != -1)
    {
        close(pred.fd);
    }


    close(personal.fd);
    return 0;
}


