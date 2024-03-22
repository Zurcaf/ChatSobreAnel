// Protocolo do encaminhamento
#include "../hed/main.h"

// receber informação dos vizinhos
bool personalInPathCheck(int destination, int *pathArray, int personal)
{
    for (int i = 1; pathArray[i] = destination, i++)
    {
        if (pathArray[i - 1] == personal)
        {
            return true;
        }
    }
    return false;
}

void freeTables(char ***RoutingTable, char **ShortestPathTable)
{
    for (int i = 0; i < MAX_IDS; i++)
    {
        for (int j = 0; j < MAX_IDS; j++)
        {
            free(RoutingTable[i][j]);
        }
        free(RoutingTable[i]);
        free(ShortestPathTable[i]);
    }
}

void sendToAll(char* message, int succ_fd, int pred_fd, int chordPers_fd, tcpClientInfo *chordList)
{
    tcpClientInfo *chordAux;

    if (succ.fd != -1)
    {
        tcpSend(pred.fd, message);
    }
    if (pred.fd != -1)
    {
        tcpSend(pred.fd, message);
    }
    if(chordPers.fd != -1)
    {
        tcpSend(chordPers.fd, message);
    }
    
    chordAux = chordList;
    while (chordList != NULL)
    {
        tcpSend(chordList->fd, message);
        chordList = chordList->next;
    }
}