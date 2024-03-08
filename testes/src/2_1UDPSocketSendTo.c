// Código para enviar uma mensagem UDP para um servidor e receber a resposta.
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

int main(void) {
    int fd;
    int errcode;
    char buffer[128 + 1];

    struct addrinfo hints, *res;
    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("Error creating socket");
        exit(1);
    }

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;  // IPv4
    hints.ai_socktype = SOCK_DGRAM;  // UDP socket

    errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", "58001", &hints, &res);
    if (errcode != 0) {
        perror("Error connecting");
        exit(1);
    }

    n = sendto(fd, "Hello!\n", 7, 0, res->ai_addr, res->ai_addrlen);
    if (n == -1) {
        perror("Error messaging");
        exit(1);
    }

    freeaddrinfo(res);

    // Agora, o socket está pronto para leitura
    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 128, 0, &addr, &addrlen);
    if (n == -1) {
        perror("Error receiving data");
        exit(1);
    }

    buffer[n] = '\0';
    printf("echo: %s\n", buffer);

    close(fd);

    exit(0);
}