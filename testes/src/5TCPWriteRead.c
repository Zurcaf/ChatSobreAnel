//TCP, write and read
#include <unistd.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(void)
{
    int fd;
    ssize_t nbytes, nleft, nwritten, nread;
    char *ptr, buffer[128 + 1];



    struct addrinfo hints, *res;
    int n;

    fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    if (fd == -1)
        exit(1); // error

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    n = getaddrinfo("tejo.tecnico.ulisboa.pt", "58001", &hints, &res);
    if (n != 0) /*error*/
        exit(1);
    
    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) /*error*/
        exit(1);



    /*...*/ // see previous task code
    ptr = strcpy(buffer, "Hello!\n");

    nbytes = 7;
    nleft = nbytes;

    while (nleft > 0)
    {
        nwritten = write(fd, ptr, nleft);
        if (nwritten <= 0) /*error*/
            exit(1);
        nleft -= nwritten;
        ptr += nwritten;
    }

    nleft = nbytes;
    ptr = buffer;

    while (nleft > 0)
    {
        nread = read(fd, ptr, nleft);
        if (nread == -1) /*error*/
            exit(1);
        else if (nread == 0)
            break; // closed by peer
        nleft -= nread;
        ptr += nread;
    }
    nread = nbytes - nleft;

    buffer[nread] = '\0';

    printf("echo: %s\n", buffer);
    close(fd);
    exit(0);
}