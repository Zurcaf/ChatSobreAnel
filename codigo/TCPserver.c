#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>


int main(void)
{
    printf("hello\n");
    struct addrinfo hints, *res;
    int fd, newfd, errcode;
    ssize_t n, nw;
    struct sockaddr addr;
    socklen_t addrlen;
    char *ptr, buffer[128];
    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit(1); // error
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE;
    if ((errcode = getaddrinfo("127.0.0.1", "58002", &hints, &res)) != 0) /*error*/
        exit(1);
    if (bind(fd, res->ai_addr, res->ai_addrlen) == -1) {
        printf("error binding");
        exit(1);
    }

    if (listen(fd, 5) == -1) /*error*/
        exit(1);
    while (1)
    {
        addrlen = sizeof(addr);
        printf("Waiting to accept connections \n");
        if ((newfd = accept(fd, &addr, &addrlen)) == -1)
            /*error*/ exit(1);


        printf("Connection accepted\n");
        while ((n = read(newfd, buffer, 128)) != 0)
        {
            if (n == -1) /*error*/
               break; //exit(1);
            ptr = &buffer[0];
            while (n > 0)
            {
                if ((nw = write(newfd, ptr, n)) <= 0) /*error*/
                    exit(1);
                n -= nw;
                ptr += nw;
                printf("%s \n", buffer);
            }
        }
        close(newfd);
    }
    // freeaddrinfo(res);close(fd);exit(0);
}