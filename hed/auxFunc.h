#ifndef AUXFUNC_H
#define AUXFUNC_H

//verificação de memoria alocada no endereço
void memoryCheck(void *ptr);

//verificar se o ficheiro foi bem aberto
void fileCheck(FILE *filePointer, char *fileName);

//inicilizar buffer
void bufferInit(char *buffer);

//inicialização dos valores das estruturas
void inicializer(ServerInfo *server, NodeInfo *personal, NodeInfo *succ, NodeInfo *succ2, NodeInfo *pred);

//inicio de uma conexão TCP (servidor)
void tcpServerInit(NodeInfo *server);

//inicio de uma conexão TCP (cliente)
void tcpClientInit(NodeInfo *client);

void tcpSend(NodeInfo dest, char *buffer);

void tcpReceive (int *newfd, NodeInfo *pred);

//envio de mensagens para o servidor (UDP)
void nodeServSend(ServerInfo server, char* buffer);

void messageTokenize(char *message, char **inputArray, int *inputCount, char delim);

//inicializar SET's dos descritores
void SETs_Init(fd_set *readfds, int *maxfd, int personal_fd, int succ_fd, int succ2_fd, int pred_fd);

//verificação de endereço IP
bool ValidIPAddress(const char *ipAddress);

//verificações dos argumentos (quantidade e extensões)
void argsCheck(int argc, char *argv[], char *IP, int *TCP, char *regIP, int *regUDP);

//verificação do canal de escuta
void listeningChanelInterpret(int *newfd, NodeInfo *pred);

#endif