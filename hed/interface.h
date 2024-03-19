#ifndef INTERFACE_H
#define INTERFACE_H

//verificação do input e separação dos argumentos
int inputCheck(char* input, int *inputCount, char** inputArray);

void newPersonalID(tcpServerInfo** nodes, tcpServerInfo *personal);

void newSuccID(tcpServerInfo** nodes, tcpServerInfo *personal, tcpServerInfo *succ);

void directJoin(tcpServerInfo personal, tcpServerInfo *Succ);

void join(tcpServerInfo *personal, tcpServerInfo *succ, tcpServerInfo *succ2, tcpClientInfo *pred, udpServer server, int ring);

void leave(int ring, tcpServerInfo personal, tcpServerInfo succ, tcpServerInfo succ2, tcpClientInfo pred, udpServer server);

void showTopology(tcpServerInfo personal, tcpServerInfo succ, tcpServerInfo succ2, tcpClientInfo pred);

#endif