#ifndef ENCAMINHAMETO_H
#define ENCAMINHAMENTO_H

bool personalInPathCheck(int destination, int *pathArray, int personal);

void freeTables(char ***RoutingTable, char **ShortestPathTable);

void sendToAll(char* message, int succ_fd, int pred_fd, int chordPers_fd, tcpClientInfo *chordList);




#endif // ENCAMINHAMENTO_H
