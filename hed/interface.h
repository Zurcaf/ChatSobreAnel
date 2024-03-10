#ifndef INTERFACE_H
#define INTERFACE_H

//verificação do input e separação dos argumentos
int inputCheck(char* input, int *inputCount, char** inputArray);

bool join(char *IP, int TCP, char *regIP, int regUDP, char *ring, char *id);

bool leave(char *IP, int TCP, char *regIP, int regUDP, char *ring, char *id);

#endif