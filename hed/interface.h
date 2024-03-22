#ifndef INTERFACE_H
#define INTERFACE_H

//verificação do input e separação dos argumentos
int inputCheck(char* input, int *inputCount, char** inputArray);

void newPersonalID(tcpServerInfo** nodes, tcpServerInfo *personal);

void newSuccID(tcpServerInfo** nodes, tcpServerInfo *personal, tcpServerInfo *succ);

void directJoin(tcpServerInfo personal, tcpServerInfo *Succ, tcpClientInfo *pred, tcpServerInfo *succ2);

void join(tcpServerInfo *personal, tcpServerInfo *succ, tcpServerInfo *succ2, tcpClientInfo *pred, udpServer server, int ring, int* nodesInRing);

void closingConnections(tcpServerInfo *succ, tcpServerInfo *succ2, tcpClientInfo *pred, tcpServerInfo *personal, tcpServerInfo *chordClient, tcpClientInfo *chordServerList);

void leave(int ring, udpServer server, tcpServerInfo *personal, tcpServerInfo *succ, tcpServerInfo *succ2, tcpClientInfo *pred, tcpServerInfo *chordClient, tcpClientInfo *chordServerList);

void showTopology(tcpServerInfo personal, tcpServerInfo succ, tcpServerInfo succ2, tcpClientInfo pred, tcpServerInfo chordClient, tcpClientInfo *chordServerList);

void chordServerInit(udpServer server, tcpClientInfo *chordList,  tcpServerInfo *chordPers, tcpClientInfo pred, tcpServerInfo succ, tcpServerInfo personal, int ring);

#endif