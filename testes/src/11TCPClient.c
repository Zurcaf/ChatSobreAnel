#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <netdb.h>

int main(void)
{
    int fd, n;
    char *ptr, buffer[128 + 1];
    struct addrinfo hints, *res;
    ssize_t nbytes, nleft, nwritten;

    fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    if (fd == -1)
        exit(1); // error

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket

    n = getaddrinfo("127.0.0.1", "58009", &hints, &res);
    if (n != 0) /*error*/
        exit(1);

    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1) /*error*/
        exit(1);

    while (1)
    {
        printf("Send message?\n");
        fgets(buffer, 128, stdin);
        
        ptr = buffer;
        nbytes = strlen(buffer);
        nleft = nbytes;

        if (strcmp(buffer, "x\n") == 0)
            break;

        while (nleft > 0)
        {
            nwritten = write(fd, ptr, nleft);
            if (nwritten <= 0) /*error*/
                exit(1);
            nleft -= nwritten;
            ptr += nwritten;
        }
        for (int i = 0; i < 128; i++)
        {
            buffer[i] = '0';
        }
        ptr = NULL;

        for (int i = 0; i < 128; i++)
            buffer[i] = '\0';

                printf("Closing connection\n");
    ssize_t ne, nw;
    while (1)
    {
    
        ne = read(fd, buffer, sizeof(buffer));
        if (ne == -1)
        {
            printf("error reading listennig chanel\n");
        }

        ptr = &buffer[0];

        while (ne > 0)
        {
            if ((nw = write(fd, ptr, ne)) <= 0)
                exit(1);
            ne -= nw;
            ptr += nw;
        }
        buffer[sizeof(buffer)] = '\0';
    }
    }

    

    close(fd);
}