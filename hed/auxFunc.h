#ifndef AUXFUNC_H
#define AUXFUNC_H

//verificação de memoria alocada no endereço
void memoryCheck(void *ptr);

//verificar se o ficheiro foi bem aberto
void fileCheck(FILE *filePointer, char *fileName);

//verificação de endereço IP
bool ValidIPAddress(const char *ipAddress);

//verificações dos argumentos (quantidade e extensões)
void argsCheck(int argc, char *argv[], char *IP, int *TCP, char *regIP, int *regUDP);

#endif