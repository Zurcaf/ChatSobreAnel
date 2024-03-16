#ifndef INTERFACE_H
#define INTERFACE_H

//verificação do input e separação dos argumentos
int inputCheck(char* input, int *inputCount, char** inputArray);

void newPersonalID(NodeInfo** nodes, NodeInfo *personal);

void newSuccID(NodeInfo** nodes, NodeInfo *personal, NodeInfo *succ);

void directJoin(NodeInfo personal, NodeInfo Succ);

void join(NodeInfo *personal, NodeInfo *succ, NodeInfo *succ2, NodeInfo *pred, ServerInfo server, int ring);

void leave(int ring, NodeInfo personal, ServerInfo server);

void showTopology(NodeInfo personal, NodeInfo succ, NodeInfo succ2, NodeInfo pred);


#endif