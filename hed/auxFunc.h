#ifndef AUXFUNC_H
#define AUXFUNC_H

//verificação de memoria alocada no endereço
void memoryCheck(void *ptr);

//verificar se o ficheiro foi bem aberto
void fileCheck(FILE *filePointer, char *fileName);

//inicilizar buffer
void bufferInit(char *buffer);

//inicialização dos valores das estruturas
void inicializer(int mode, udpServer *server, tcpServerInfo *personal, tcpServerInfo *succ, tcpServerInfo *succ2, tcpClientInfo *pred, tcpServerInfo *chordClient);

//inicio de uma conexão TCP (servidor)
void tcpServerInit(tcpServerInfo *server);

//inicio de uma conexão TCP (cliente)
void tcpClientInit(tcpServerInfo *client);

void tcpSend(int fdDest, char *buffer);

int tcpReceive (int fdRec, char *message);

//envio de mensagens para o servidor (UDP)
void nodeServSend (udpServer server, char* buffer, int counter);

void messageTokenize(char *message, char **inputArray, int *inputCount, int mode);

//inicializar SET's dos descritores
void SETs_Init(fd_set *readfds, int *maxfd, int personal_fd, int succ_fd, int succ2_fd, int pred_fd, int chord_fd, tcpClientInfo *chordServerList);

//verificação de endereço IP
bool ValidIPAddress(const char *ipAddress);

//verificações dos argumentos (quantidade e extensões)
void argsCheck(int argc, char *argv[], char *IP, int *TCP, char *regIP, int *regUDP);

//verificação do canal de escuta
int getMessageType(char* message, char** messageArray);

#endif