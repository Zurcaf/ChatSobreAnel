#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

// int function retorna zero quando da erro 1 quando a ligaçao e comunicaçao têm sucesso
// args Char[]-comando que é preciso enviar  struct node
int main(void)
{
    int fd, errcode;
    char buffer[128];

    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;
    struct addrinfo hints, *res;

    // socket creation and verification
    fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket
    if (fd == -1)                        /*error*/
        exit(1);

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket

    errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", "59000", &hints, &res);
    if (errcode != 0)
    { /*error*/
        printf("Error connecting");
        exit(1);
    }

    printf("Connected\n");

    n = sendto(fd, "RST 067", 32, 0, res->ai_addr, res->ai_addrlen);
    // n = sendto(fd, "UNREG 067 10", 32, 0, res->ai_addr, res->ai_addrlen);
    // n = sendto(fd, "UNREG 067 11", 32, 0, res->ai_addr, res->ai_addrlen);


    // n = sendto(fd, "NODES 067", 32, 0, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    { /*error*/
        printf("Error messaging.");
        exit(1);
    }

    freeaddrinfo(res);

    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 500, 0, &addr, &addrlen);
    if (n == -1) /*error*/
        exit(1);

    buffer[n] = '\0';

    printf("echo: %s\n", buffer);
    close(fd);
    exit(0);
}
