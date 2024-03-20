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
    bool oldNode = false;
    udpServer server;
    tcpServerInfo personal;
    tcpServerInfo succ;
    tcpServerInfo succ2;
    tcpClientInfo pred;
    
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

    printf("------------------------------------------------------------\n");
    printf("Application COR invoked with the following parameters:\n");
    printf("IP: %s\n", personal.IP);
    printf("TCP: %d\n", personal.TCP);
    printf("regIP: %s\n", server.regIP);
    printf("regUDP: %d\n", server.regUDP);
    printf("------------------------------------------------------------\n");
    printf("Available comands:\njoin (j) ring id\ndirect join (dj) id succid succIP succTCP\nchord (c)\nremove chord (rc)\nshow topology (st)\nshow routing (sr) dest\nshow path (sp) dest\nshow forwarding (sf)\nmessage (m) dest message\nleave (l)\nexit (x)\n");
    printf("------------------------------------------------------------\n");
    printf("Insert command: \n");

    while (1)
    {
        // Inicialização dos descritores para o select
        SETs_Init(&readfds, &maxfd, personal.fd, succ.fd, succ2.fd, pred.fd);

        // Esperar por atividade com o select
        // printf ("Espera por atividade\n");
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }

        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            // printf("stdin is set\n");
            fgets(input, sizeof(input), stdin);

            command = inputCheck(input, &argumentCount, arguments);

            switch (command)
            {
                case 0:
                    break;
                case 1:
                    if (oldNode)
                    {
                        printf("You are already in a ring\n");
                        break;
                    }
                    ring = atoi(arguments[1]);
                    personal.id = atoi(arguments[2]);
                    join(&personal, &succ, &succ2, &pred, server, ring);
                    break;
                case 2:
                    if (oldNode)
                    {
                        printf("You are already in a ring\n");
                        break;
                    }
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
                    if (oldNode)
                    {
                        printf("You are not in a ring\n");
                        break;
                    }
                    leave(ring, &personal, &succ, &succ2, &pred, server);
                    oldNode = false;
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
            // printf("Canal de escuta is set\n");

            // Novas conexões no socket
            if ((newfd = accept(personal.fd, NULL, NULL)) == -1) 
                exit(1); /* error */
            
            tcpReceive(newfd, message);
            messageArray = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
            memoryCheck(messageArray);

            messageType = getMessageType(message, messageArray);


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

                        succ.id= atoi(messageArray[1]);
                        strcpy(succ.IP, messageArray[2]);
                        succ.TCP = atoi(messageArray[3]);

                        tcpClientInit(&succ);
                        bufferInit(message);
                        
                        sprintf(message, "PRED %02d\n", personal.id);
                        tcpSend(succ.fd, message);
                    }
                    else
                    {
                        tcpSend(pred.fd, message);

                        pred.fd = newfd;
                        pred.id = atoi(messageArray[1]);

                        sprintf(message, "SUCC %02d %s %05d\n", succ.id, succ.IP, succ.TCP);
                        tcpSend(pred.fd, message);
                    }
                    break;

                //caso seja um PRED
                case 2:
                    pred.fd = newfd;
                    pred.id = atoi(messageArray[1]);
                    if (oldNode)
                    {
                        oldNode = true;
                    }
                    else
                    {
                        bufferInit(message);
                        sprintf(message, "SUCC %02d %s %05d\n", succ.id, succ.IP, succ.TCP);
                        tcpSend(pred.fd, message);
                    }
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
            // printf("SUCC is set\n");

            aux = tcpReceive(succ.fd, message);

            if (aux == 0)
            {
                printf("Canal de sucessor fechado\n");
                if(succ.fd != -1)
                {
                    close(succ.fd);
                    succ.fd = -1;
                }

                if (succ2.id == personal.id)
                {
                    //ficamos no anel sozinhos
                    succ.id = personal.id;
                }
                else
                {
                    //garantir que o anel não se separa
                    succ.id = succ2.id;
                    strcpy(succ.IP, succ2.IP);
                    succ.TCP = succ2.TCP;

                    tcpClientInit(&succ);

                    bufferInit(message);
                    sprintf(message, "PRED %02d \n", personal.id);
                    tcpSend(succ.fd, message);

                    bufferInit(message);
                    sprintf(message, "SUCC %02d %s %05d\n", succ.id, succ.IP, succ.TCP);
                    tcpSend(pred.fd, message);
                }

            }
            else
            {
                messageArray = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
                memoryCheck(messageArray);

                messageType = getMessageType(message, messageArray);

                switch (messageType)
                {
                    case 0:
                        printf("Error in the successor channel (mensagem não reconhecida)\n"); 
                        break;
                    //Recebemos um ENTRY
                    case 1:

                        //Antigo sucessor passa a ser o segundo sucessor
                        succ2.id = succ.id;
                        strcpy(succ2.IP, succ.IP);
                        succ2.TCP = succ.TCP;

                        if (succ.fd != -1)
                        {
                            close(succ.fd);
                            succ.fd = -1;
                        }

                        //Mensagem recebida dá nos a info do novo sucessor
                        succ.id = atoi(messageArray[1]);
                        strcpy(succ.IP, messageArray[2]);
                        succ.TCP = atoi(messageArray[3]);
                        tcpClientInit(&succ);

                        //Envio de um SUCC para o predecessor
                        bufferInit(message);
                        sprintf(message, "SUCC %02d %s %05d\n", succ.id, succ.IP, succ.TCP);
                        tcpSend(pred.fd, message);

                        //Envio de um PRED para o novo sucessor
                        bufferInit(message);
                        sprintf(message, "PRED %02d\n", personal.id);
                        tcpSend(succ.fd, message);
                        break;
                    
                    //Recebemos um PRED
                    case 3:
                        succ2.id = atoi(messageArray[1]);
                        strcpy(succ2.IP, messageArray[2]);
                        succ2.TCP = atoi(messageArray[3]);
                        
                        break;
                    case 4:
                        break;
                    default:
                        break;
                }

                for(int i = 0; messageArray[i] != NULL; i++)
                {
                    free(messageArray[i]);
                }
                free(messageArray);
                messageType = 0;
            }
        }

        if (FD_ISSET(pred.fd, &readfds))
        {
            // printf("PRED is set\n");

            aux = tcpReceive(pred.fd, message);
            if (aux == 0)
            {
                printf("Canal do predecessor fechado\n");
                if(pred.fd!= -1)
                {
                    close(pred.fd);
                    pred.fd = -1;
                }
                pred.id = personal.id;

            }
            else
            {
                messageArray = (char **)malloc(MAX_ARGUMENTS * sizeof(char *));
                memoryCheck(messageArray);

                messageType = getMessageType(message, messageArray);

                switch (messageType)
                {
                    case 0:
                        printf("Error in the predecessor channel (mensagem não reconhecida)\n"); 
                        break;
                    default:
                        break;
                }

                for(int i = 0; messageArray[i] != NULL; i++)
                {
                    free(messageArray[i]);
                }
                free(messageArray);
                messageType = 0;

            }
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


