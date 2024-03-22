// Protocolo do encaminhamento
#include "../hed/main.h"
#include "../hed/encaminhamento.h"
#include "../hed/interface.h"
#include "../hed/auxFunc.h"

// receber informação dos vizinhos
bool personalInPathCheck(int destination, int *pathArray, int personal)
{
    int i = 0;
    while (pathArray[i] != personal)
    {
        if (pathArray[i] == destination)
        {
            return false;
        }
        i++;
    }
    return true;
}

void freeTables(char ***RoutingTable, char **ShortestPathTable)
{
    for (int i = 0; i < MAX_IDS; i++)
    {
        if (RoutingTable[i] == NULL)
        {
            continue;
        }
        for (int j = 0; j < MAX_IDS; j++)
        {
            if (RoutingTable[i][j] == NULL)
            {
                continue;
            }
            free(RoutingTable[i][j]);
        }
        free(RoutingTable[i]);
        free(ShortestPathTable[i]);
    }
}

void sendToAll(char* message, int succ_fd, int pred_fd, int chordPers_fd, tcpClientInfo *chordList)
{
    tcpClientInfo *chordAux = NULL;

    if (succ_fd != -1)
    {
        tcpSend(pred_fd, message);
    }
    if (pred_fd != -1)
    {
        tcpSend(pred_fd, message);
    }
    if(chordPers_fd != -1)
    {
        tcpSend(chordPers_fd, message);
    }

    chordAux = chordList;
    while (chordList != NULL)
    {
        tcpSend(chordAux->fd, message);
        chordAux = chordAux->next;
    }
}