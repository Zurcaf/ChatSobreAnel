#ifndef INTERFACE_H
#define INTERFACE_H

//verificação do input e separação dos argumentos
int inputCheck(char* input, int *inputCount, char** inputArray);

bool directJoin();

bool join(NodeInfo personal, ServerInfo server, char** arguments);

bool leave();

#endif