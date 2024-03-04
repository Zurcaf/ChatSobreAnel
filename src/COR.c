//Invocação da aplicação ChatOnRing
// Aqui localizam-se o main do programa, onde são invocadas as funções da aplicação ChatOnRing.

#include "../hed/COR.h"

//Invocação da aplicação pelo comando:
// COR IP TCP regIP regUDP
// IP é o endereço IP da máquina
// TCP é o porto TCP onde a máquina vai escutar
int main(int argc, char* argv[])
{
char IP[MAX_IP_LENGTH + 1];
int TCP = 0;

char regIP[MAX_IP_LENGTH + 1] = "193.136.138.142";
int regUDP = 59000;

argsCheck(argc, argv, IP, &TCP, regIP, &regUDP);

char input[256]; // Maximum input length

printf("\nApplication COR invoked with the following parameters:\n");
printf("IP: %s\n", IP);
printf("TCP: %d\n", TCP);
printf("regIP: %s\n", regIP);
printf("regUDP: %d\n", regUDP);
printf("Available comands:\njoin (j) ring id\ndirect join (dj) id succid succIP succTCP\nchord (c)\nremove chord (rc)\nshow topology (st)\nshow routing (sr) dest\nshow path (sp) dest\nshow forwarding (sf)\nmessage (m) dest message\nleave (l)\nexit (x)\n");

while (1)
{
    scanf("%s", input);
}

return 0;
}

