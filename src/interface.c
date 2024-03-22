#include "../hed/main.h"
#include "../hed/auxFunc.h"

int inputCheck(char* input, int *inputCount, char** inputArray)
{
    *inputCount = 0;

    messageTokenize(input, inputArray, inputCount, 1);

    if (strcmp(inputArray[0], "join") == 0 || strcmp(inputArray[0], "j") == 0)
    {
        // verificação do numero de argumentos para o join
        if (*inputCount != 3)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for join command!\n");
            return (0);
        }

        // verificação do formato dos argumentos para o join
        if (strlen(inputArray[1]) == 3 && strlen(inputArray[2]) == 2)
        {
            if ((isdigit(inputArray[1][0]) && isdigit(inputArray[1][1]) && isdigit(inputArray[1][2])))
            {
                if (isdigit(inputArray[2][0]) && isdigit(inputArray[2][1]))
                    return (1);
            }
        }
        
        fprintf(stderr, "ERROR: wrong format of the join arguments!\n");
        fprintf(stderr, "\tring formate: 000\n");
        fprintf(stderr, "\tid formate: 00\n");
        return (0);
    }

    if (((strcmp(inputArray[0], "direct") == 0) && strcmp(inputArray[1], "join") == 0) || strcmp(inputArray[0], "dj") == 0)
    {
        if (*inputCount != 5 && *inputCount != 6)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for direct join command!\n");
            return (0);
        }
        if (strlen(inputArray[2]) == 2 && strlen(inputArray[3]) == 2 && strlen(inputArray[4]) == MAX_IP_LENGTH && strlen(inputArray[5]) == 5)
        {
            if (isdigit(inputArray[2][0]) && isdigit(inputArray[2][1]))
            {
                if (isdigit(inputArray[3][0]) && isdigit(inputArray[3][1]))
                {
                    if (ValidIPAddress(inputArray[4]) == false)
                    {
                        if (atoi(inputArray[5]) > 0 && atoi(inputArray[5]) < 65536)
                            return (2);
                    }
                }
            }
        }
        fprintf(stderr, "ERROR: wrong format of the direct join arguments!\n");
        fprintf(stderr, "\tid formate: 00\n");
        fprintf(stderr, "\tSucc id formate: 00\n");
        fprintf(stderr, "\tSucc IP formate: 000.000.000.000\n");
        fprintf(stderr, "\tSucc Port formate: 00000\n");

        return 0;

    }
    
    if (strcmp(inputArray[0], "chord") == 0 || strcmp(inputArray[0], "c") == 0)
    {
        if (*inputCount != 1)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for chord command!\n");
            return (0);
        }
        return (3);
    }

    if (((strcmp(inputArray[0], "remove") == 0) && strcmp(inputArray[1], "chord") == 0) || strcmp(inputArray[0], "rc") == 0)
    {
        if (*inputCount != 1)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for remove command!\n");
            return (0);
        }
        return (4);
    }

    if (((strcmp(inputArray[0], "show") == 0) && strcmp(inputArray[1], "topology") == 0) || strcmp(inputArray[0], "st") == 0)
    {
        if (*inputCount != 1)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for remove command!\n");
            return (0);
        }
        return (5);
    }

    if (((strcmp(inputArray[0], "show") == 0) && strcmp(inputArray[1], "routing") == 0) || strcmp(inputArray[0], "sr") == 0)
    {
        if (*inputCount != 2)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for remove command!\n");
            return (0);
        }
        return (6);
    }

    if(((strcmp(inputArray[0], "show") == 0) && strcmp(inputArray[1], "path") == 0) || strcmp(inputArray[0], "sp") == 0)
    {
        if (*inputCount != 2)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for remove command!\n");
            return (0);
        }
        return (7);
    }

    if (((strcmp(inputArray[0], "show") == 0) && strcmp(inputArray[1], "forwarding") == 0) || strcmp(inputArray[0], "sf") == 0)
    {
        if (*inputCount != 1)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for remove command!\n");
            return (0);
        }
        return (8);
    }

    if(strcmp(inputArray[0], "message") == 0 || strcmp(inputArray[0], "m") == 0)
    {
        if (*inputCount != 3)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for message command!\n");
            return (0);
        }
        return (9);
    }

    if(strcmp(inputArray[0], "leave") == 0 || strcmp(inputArray[0], "l") == 0)
    {
        if (*inputCount != 1)
        {
            fprintf(stderr, "ERROR: wrong number of arguments for leave command!\n");
            return (0);
        }
        return (10);
    }

    if (strcmp(inputArray[0], "exit") == 0 || strcmp(inputArray[0], "x") == 0)
    {
        printf("Exiting the application...\n");
        return (11);
    }
    
    fprintf(stderr, "ERROR: command not found!\n\n");
    
    return 0;
}

void newPersonalID(tcpServerInfo** nodes, tcpServerInfo *personal)
{

    if (nodes[personal->id]->id != -1)
    {
        for (int i = personal->id; i < MAX_IDS; i++)
        {
            if (nodes[i]->id == -1)
            {
                personal->id = i;
                break;
            }
        }
        printf("ID already in use. New ID: %02d\n", personal->id);
    }
    if (nodes[personal->id]->id != -1)
    {
        for (int i = 0; i < personal->id; i++)
        {
            if (nodes[i]->id == -1)
            {
                personal->id = i;
                break;
            }
        }
        printf("ID already in use. New ID: %02d\n", personal->id);
    }

    return;
}

void newSuccID(tcpServerInfo** nodes, tcpServerInfo *personal, tcpServerInfo *succ)
{
    //encontrar o sucessor
    if (nodes[succ->id]->id == -1)
    {
        for (int i = succ->id; i < MAX_IDS; i++)
        {
            if (nodes[i]->id != -1)
            {
                succ->id = i;
                break;
            }
        }
    }

    if (nodes[succ->id]->id == -1)
    {    
        for (int i = 0; i < personal->id; i++)
        {
            if (nodes[i]->id != -1)
            {
                succ->id = i;
                break;
            }
        }
    }

    strcpy(succ->IP, nodes[succ->id]->IP);
    succ->TCP = nodes[succ->id]->TCP;
}

void directJoin(tcpServerInfo personal, tcpServerInfo* Succ)
{
    char message[MAX_BUFFER];

    //inicializar buffer
    bufferInit(message);

    tcpClientInit(Succ);

    sprintf(message, "ENTRY %02d %s %05d\n", personal.id, personal.IP, personal.TCP);

    tcpSend(Succ->fd, message);

    return;
}

//Implementação dos comandos da interface com o utlizador (4.2)
void join(tcpServerInfo *personal, tcpServerInfo *succ, tcpServerInfo *succ2, tcpClientInfo *pred, udpServer server, int ring, int* nodesInRing)
{
    int lineCounter = 0;
    int infoCounter = 0;

    char message[MAX_RESPONSE];

    //inicializar buffer
    bufferInit(message);

    sprintf(message, "NODES %03d", ring);
    nodeServSend(server, message, 0);

    //alocar memoria para um vetor de linhas
    char **lines = (char **)calloc(MAX_IDS, sizeof(char *));
    memoryCheck(lines);

    messageTokenize (message, lines, &lineCounter, 0);

    char **information = (char **)calloc(MAX_IDS, sizeof(char *));
    memoryCheck(information);

    tcpServerInfo **nodes = (tcpServerInfo **)calloc(MAX_IDS, sizeof(tcpServerInfo*));
    memoryCheck(nodes);

    for (int i = 0; i < MAX_IDS; i++)
    {
        nodes[i] = (tcpServerInfo *)calloc(1, sizeof(tcpServerInfo));
        memoryCheck(nodes[i]);

        nodes[i]->id = -1;
    }

    free(lines[0]);

    for (int i = 1; i < lineCounter; i++)
    {
        messageTokenize(lines[i], information, &infoCounter, 1);

        nodes[atoi(information[0])]->id = atoi(information[0]);
        strcpy(nodes[atoi(information[0])]->IP, information[1]);
        nodes[atoi(information[0])]->TCP = atoi(information[2]);

        for (int j = 0; j < infoCounter; j++)
        {
            free(information[j]);
        }

        free(lines[i]);
        infoCounter = 0;
    }
    free(information);
    free(lines);

    *nodesInRing = lineCounter-1;

    // Confirmar que o ID do nó não está a ser usado
    newPersonalID(nodes, personal);

    //inicializar ID's dos nós
    succ->id = personal->id;
    pred->id = personal->id;
    succ2->id = personal->id;

    //encontrar o proximo nó
    if (lineCounter > 1)
    {
        succ->id = personal->id + 1;
        newSuccID(nodes, personal, succ);

        //call direct join
        directJoin(*personal, succ);
    }
    
    for (int i = 0; i < MAX_IDS; i++)
    {
        free(nodes[i]);
    }

    free(nodes);

    // Registo do nó no servidor
    bufferInit(message);
    sprintf(message, "REG %03d %02d %s %05d", ring, personal->id, personal->IP, personal->TCP);
    
    nodeServSend(server, message, 0);
    printf("%s\n", message);

    return;
}

void closingConnections(tcpServerInfo *succ, tcpServerInfo *succ2, tcpClientInfo *pred, tcpServerInfo *personal, tcpServerInfo *chordClient, tcpClientInfo *chordServerList)
{
    tcpClientInfo *aux;
    while (chordServerList != NULL)
    {
        aux = chordServerList;
        close(aux->fd);

        chordServerList = chordServerList->next;
        free(aux);
    }

    if(chordClient->fd != -1)
    {
        close(chordClient->fd);
    }

    if(succ->fd != -1)
    {
        close(succ->fd);
    }

    if(succ2->fd != -1)
    {
        close(succ2->fd);
    }

    if(pred->fd != -1)
    {
        close(pred->fd);
    }

    if (personal->fd != -1)
    {
        close(personal->fd);
    }
    printf("FD: -%d-", personal->fd);
}

void leave(int ring, udpServer server, tcpServerInfo *personal, tcpServerInfo *succ, tcpServerInfo *succ2, tcpClientInfo *pred, tcpServerInfo *chordClient, tcpClientInfo *chordServerList)
{
    char message[MAX_BUFFER];

    //inicializar buffer
    bufferInit(message);

    sprintf(message, "UNREG %03d %02d", ring, personal->id);
    nodeServSend(server, message, 0);

    //Anel Unitário (só sair)
    if (succ->fd == personal->id)
    {
        return;
    } 

    closingConnections(succ, succ2, pred, personal, chordClient, chordServerList);
    
    inicializer(1, NULL, personal, succ, succ2, pred, chordClient);
    
    return;
}

void showTopology(tcpServerInfo personal, tcpServerInfo succ, tcpServerInfo succ2, tcpClientInfo pred, tcpServerInfo chordClient, tcpClientInfo *chordServerList)
{
    int chordsCount = 0;
    tcpClientInfo *aux = chordServerList;
    printf("------------------------------------------------------------\n");
    printf("Pred      ID%02d\n",  pred.id);
    printf("Personal  ID%02d IP%s TCP%05d\n",  personal.id, personal.IP, personal.TCP);
    printf("Succ      ID%02d IP%s TCP%05d\n",  succ.id, succ.IP, succ.TCP);
    printf("Succ2     ID%02d IP%s TCP%05d\n",  succ2.id, succ2.IP, succ2.TCP);
    printf("Chord%d    ID%02d IP%s TCP%05d\n",  chordsCount,chordClient.id, chordClient.IP, chordClient.TCP);
    chordsCount++;
    
    while (aux != NULL)
    {
        printf("Chord%d    ID%02d\n",  chordsCount, aux->id);
        chordsCount++;
        aux = aux->next;
    }
    printf("------------------------------------------------------------\n");
    printf("\n");
    return;
}

void chordServerInit(udpServer server, tcpClientInfo *chordList,  tcpServerInfo *chordPers, tcpClientInfo pred, tcpServerInfo succ, tcpServerInfo personal, int ring)
{
    int infoCounter = 0;
    int lineCounter = 0;

    char message[MAX_RESPONSE];

    //inicializar buffer
    bufferInit(message);

    sprintf(message, "NODES %03d", ring);
    nodeServSend(server, message, 0);

    //alocar memoria para um vetor de linhas
    char **lines = (char **)calloc(MAX_IDS, sizeof(char *));
    memoryCheck(lines);

    messageTokenize (message, lines, &lineCounter, 0);

    char **information = (char **)calloc(MAX_IDS, sizeof(char *));
    memoryCheck(information);

    tcpServerInfo **nodes = (tcpServerInfo **)calloc(MAX_IDS, sizeof(tcpServerInfo*));
    memoryCheck(nodes);

    for (int i = 0; i < MAX_IDS; i++)
    {
        nodes[i] = (tcpServerInfo *)calloc(1, sizeof(tcpServerInfo));
        memoryCheck(nodes[i]);

        nodes[i]->id = -1;
    }

    free(lines[0]);

    for (int i = 1; i < lineCounter; i++)
    {
        messageTokenize(lines[i], information, &infoCounter, 1);

        nodes[i]->id = atoi(information[0]);
        strcpy(nodes[i]->IP, information[1]);
        nodes[i]->TCP = atoi(information[2]);

        for (int j = 0; j < infoCounter; j++)
        {
            free(information[j]);
        }

        free(lines[i]);
        infoCounter = 0;
    }
    free(information);
    free(lines);

    bool flag =false;
    tcpClientInfo *aux = chordList;

    for (int i=0; i<lineCounter; i++)
    {
        if (nodes[i]->id == succ.id)
        {
            continue;    
        }
        if (nodes[i]->id == pred.id)
        {
            continue;
        }
        if (nodes[i]->id == personal.id)
        {
            continue;
        }
        
        while(aux != NULL)
        {
            if (nodes[i]->id == aux->id)
            {
                flag = true;
            }
            if (flag)
                break;
            
            aux = aux->next;
        }

        if (flag)
            continue;
        
        chordPers->id = nodes[i]->id;
        strcpy(chordPers->IP, nodes[i]->IP);
        chordPers->TCP = nodes[i]->TCP;
    }

    if (chordPers->id == -1)
    {
        printf("No available Nodes to make a Chord");
        return;
    }

    tcpClientInit(chordPers);
    
    char message2[MAX_BUFFER];
    bufferInit(message2);

    sprintf(message2, "CHORD %02d\n", personal.id);
    tcpSend(chordPers->fd, message2);


    for (int i = 0; i < MAX_IDS; i++)
    {
        free(nodes[i]);
    }
    free(nodes);

    return;
}
