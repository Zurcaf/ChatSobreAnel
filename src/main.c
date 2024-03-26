// Invocação da aplicação ChatOnRing
//  Aqui localizam-se o main do programa, onde são invocadas as funções da aplicação ChatOnRing.
#include "../hed/main.h"
#include "../hed/auxFunc.h"
#include "../hed/interface.h"
#include "../hed/encaminhamento.h"

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
    int ring = -1, nodesInRing=0;
    bool newNode = true;
    bool running = true;
    bool chordExit = false;
    udpServer server;
    tcpServerInfo personal;
    tcpServerInfo succ;
    tcpServerInfo succ2;
    tcpClientInfo pred;
    
    tcpServerInfo chordPers;
    tcpClientInfo *chordList = NULL, *chordAux = NULL, *chordAux2 = NULL;

    int ***tables = NULL;
    int neighborsFd[MAX_NODES];

    // ignorar o sinal SIGPIPE
    struct sigaction act;

    memset(&act,0,sizeof act);
    act.sa_handler=SIG_IGN;
    if(sigaction(SIGPIPE,&act,NULL)==-1)/*error*/exit(1);

    
    // Inicialização das estruturas
    inicializer(0, &server, &personal, &succ, &succ2, &pred, &chordPers);

    // Verifing the arguments passed to the application
    argsCheck(argc, argv, personal.IP, &personal.TCP, server.regIP, &server.regUDP);

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

    while (running)
    {
        // Inicialização dos descritores para o select
        SETs_Init(&readfds, &maxfd, personal.fd, succ.fd, pred.fd, chordPers.fd, chordList, neighborsFd);

        // Esperar por atividade com o select
        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1)
        {
            perror("select");
            exit(4);
        }

        if (FD_ISSET(STDIN_FILENO, &readfds))
        {
            // Alocação de memória para os argumentos
            arguments = (char **)calloc(MAX_ARGUMENTS, sizeof(char *));
            memoryCheck(arguments);

            // printf("stdin is set\n");
            fgets(input, sizeof(input), stdin);

            command = inputCheck(input, &argumentCount, arguments);

            switch (command)
            {
                case 1:
                    if (!newNode)
                    {
                        printf("You are already in a ring\n");
                        break;
                    }
                    
                    // Inicialização do servidorTCP de escuta do nó
                    tcpServerInit(&personal);

                    // passagem de argumentos para a função join
                    ring = atoi(arguments[1]);
                    personal.id = atoi(arguments[2]);

                    join(&personal, &succ, &succ2, &pred, server, ring, &nodesInRing);
                    
                    initTables(&tables);
                    initPersonalpath(tables, personal.id);

                    if (nodesInRing == 0)
                    {
                        newNode = false;
                        break;
                    }
                    
                    routingSendAll (tables, succ.fd);

                    break;
                case 2:
                    if (!newNode)
                    {
                        printf("You are already in a ring\n");
                        break;
                    }
                    personal.id = atoi(arguments[1]);
                    succ.id = atoi(arguments[2]);
                    strcpy(succ.IP, arguments[3]);
                    succ.TCP = atoi(arguments[4]);
                    directJoin(personal, &succ, &pred, &succ2);

                    initTables(&tables);
                    initPersonalpath(tables, personal.id);
                    newNode = false;
                    break;
                case 3:
                    chordServerInit(server, chordList, &chordPers, pred, succ, personal, ring);
                    break;
                case 4:
                    close(chordPers.fd);
                    chordPers.fd = -1;
                    chordPers.id = -1;
                    chordPers.TCP = -1;
                    strcpy(chordPers.IP, INIT_IP);
                    break;
                case 5:
                    showTopology(personal, succ, succ2, pred, chordPers, chordList);
                    break;
                case 6:
                    showRouting(tables, atoi(arguments[1]));
                    break;
                case 7:
                    showPath(tables, atoi(arguments[1]));
                    break;
                case 8:
                    showForwarding(tables, personal.id);
                    break;
                case 9:
                    //message();
                    break;
                case 10:
                    if (newNode)
                    {
                        printf("You are not in a ring\n");
                        break;
                    }
                    else
                    {
                    leave(ring, server, &personal, &succ, &succ2, &pred, &chordPers, chordList);
                    freeTables(tables);
                    newNode = true;
                    }
                    break;
                case 11:
                    if (!newNode)
                    {
                        leave(ring, server, &personal, &succ, &succ2, &pred, &chordPers, chordList);
                        freeTables(tables);                
                    }
                    running=false;
                    break;
            }

            for (int i = 0; arguments[i] != NULL; ++i)
            {
                free(arguments[i]);
            }
            free(arguments);
            command = 0;

            if (running)
            {
                printf("Insert command: \n");
            }
        }

        if (FD_ISSET(personal.fd, &readfds))
        {
            printf("Canal de escuta is set\n");

            // Novas conexões no socket
            if ((newfd = accept(personal.fd, NULL, NULL)) == -1) 
                exit(1); /* error */
            
            tcpReceive(newfd, message);
            messageArray = (char **)calloc(MAX_ARGUMENTS, sizeof(char *));
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

                    routingSendAll(tables, pred.fd);
                    
                    break;

                //caso seja um PRED
                case 2:
                    pred.fd = newfd;
                    pred.id = atoi(messageArray[1]);
                    if (newNode)
                    {
                        newNode = false;
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
                    chordAux = (tcpClientInfo *)calloc(1, sizeof(tcpClientInfo));
                    memoryCheck(chordAux);

                    chordAux->id = atoi(messageArray[1]);\
                    chordAux->fd = newfd;
                    chordAux->next = NULL;

                    chordAux->next = chordList;
                    chordList = chordAux;
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
                messageArray = (char **)calloc(MAX_ARGUMENTS, sizeof(char *));
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
                    //recebemos um ROUTE
                    case 5:
                        routingRecive(tables, messageArray[3], atoi(messageArray[1]), atoi(messageArray[2]), personal.id);

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
            printf("PRED is set\n");

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
                messageArray = (char **)calloc(MAX_ARGUMENTS, sizeof(char *));
                memoryCheck(messageArray);

                messageType = getMessageType(message, messageArray);

                switch (messageType)
                {
                    case 0:
                        printf("Error in the predecessor channel (mensagem não reconhecida)\n"); 
                        break;
                    //caso seja um ROUTE
                    case 5:
                        routingRecive(tables, messageArray[3], atoi(messageArray[1]), atoi(messageArray[2]), personal.id);
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

        if (FD_ISSET(chordPers.fd, &readfds))
        {
            // printf("Chord is set\n");
            aux = tcpReceive(chordPers.fd, message);
            if (aux == 0)
            {
                printf("Canal do chord fechado\n");
                if(chordPers.fd!= -1)
                {
                    close(chordPers.fd);
                    chordPers.fd = -1;
                    chordPers.id = -1;
                    chordPers.TCP = -1;
                    strcpy(chordPers.IP, INIT_IP);
                }
            }
            else
            {
                messageArray = (char **)calloc(MAX_ARGUMENTS, sizeof(char *));
                memoryCheck(messageArray);

                messageType = getMessageType(message, messageArray);

                switch (messageType)
                {
                    case 0:
                        printf("Error in the chord channel (mensagem não reconhecida)\n"); 
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

        chordAux2 = NULL;
        chordAux = chordList;
        chordExit = false;

        while(chordAux != NULL)
        {
            if (FD_ISSET(chordAux->fd, &readfds))
            {
                // printf("Chord is set\n");

                aux = tcpReceive(chordAux->fd, message);
                if (aux == 0)
                {
                    printf("Canal do chord fechado\n");
                    chordExit = true;

                    if(chordAux->fd != -1)
                    {
                        close(chordAux->fd);
                        chordAux->fd = -1;
                    }
                }
                else
                {
                    messageArray = (char **)calloc(MAX_ARGUMENTS, sizeof(char *));
                    memoryCheck(messageArray);

                    messageType = getMessageType(message, messageArray);

                    switch (messageType)
                    {
                        case 0:
                            printf("Error in the chord channel (mensagem não reconhecida)\n"); 
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
            
            if(chordExit)
            {
                if (chordAux2 != NULL)
                {
                    chordAux2->next = chordAux->next;
                    free(chordAux);
                    chordAux = chordAux2->next;
                }
                else
                {
                    chordList = chordAux->next;
                    free(chordAux);
                    chordAux = chordList;
                }
            }
            else
            {
                chordAux2 = chordAux;
                chordAux = chordAux->next;
            }
        }
    }

    return 0;
}


