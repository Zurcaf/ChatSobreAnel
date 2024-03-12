#ifndef INTERFACE_H
#define INTERFACE_H

//verificação do input e separação dos argumentos
int inputCheck(char* input, int *inputCount, char** inputArray);

bool directJoin(NodeInfo personal, NodeInfo Succ);

void newID (char* buffer, NodeInfo *personal);

bool join(NodeInfo personal, ServerInfo server, int ring);

bool directJoin(NodeInfo personal, NodeInfo Succ);

bool leave(int ring, NodeInfo personal, ServerInfo server);

#endif