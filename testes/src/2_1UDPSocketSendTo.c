// Código para enviar uma mensagem UDP para um servidor e esperar por uma resposta
// Select usado para timeout
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

int main(void) 
{
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

    //Para usar select
    fd_set read_fds;
    struct timeval timeout;

    // Configurando o conjunto de descritores de arquivo para monitorar leitura no socket
    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);

    // Configurando o timeout para 5 segundos
    timeout.tv_sec = 5;
    timeout.tv_usec = 0;

    // Usando select para esperar até que o socket esteja pronto para leitura
    int selectResult = select(fd + 1, &read_fds, NULL, NULL, &timeout);

    if (selectResult == -1) {
        perror("Error in select");
        exit(1);

    } else if (selectResult == 0) {
        // Timeout expirado
        printf("Timeout occurred. No data received.\n");
        exit(0);
    }

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