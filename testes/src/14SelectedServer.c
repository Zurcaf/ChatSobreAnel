#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>

int main(void)
{
    int fd, newfd, errcode;
    char *ptr, buffer[128];
    
    struct addrinfo hints, *res;
    ssize_t n, nw;
    struct sockaddr addr;
    socklen_t addrlen;

    fd_set readfds;
    int maxfd;

    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
        exit(1); // error
    
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    hints.ai_flags = AI_PASSIVE;
    if ((errcode = getaddrinfo("127.0.0.1", "58018", &hints, &res)) != 0)
        exit(1); /* error */

    if (bind(fd, res->ai_addr, res->ai_addrlen) == -1)
    {
        printf("error binding");
        exit(1);
    }

    if (listen(fd, 5) == -1)
        exit(1); /* error */

    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(fd, &readfds);  // Adiciona o descritor do socket
        FD_SET(STDIN_FILENO, &readfds);  // Adiciona o descritor de entrada padrão (stdin)

        maxfd = (fd > STDIN_FILENO) ? fd : STDIN_FILENO;

        printf("Waiting for activity on socket or stdin...\n");

        if (select(maxfd + 1, &readfds, NULL, NULL, NULL) == -1)
            exit(1); /* error */

        if (FD_ISSET(fd, &readfds)) 
        {
            // Novas conexões no socket
            addrlen = sizeof(addr);
            printf("Waiting to accept connections \n");
            if ((newfd = accept(fd, &addr, &addrlen)) == -1)
                exit(1); /* error */

            printf("Connection accepted\n");
            while ((n = read(newfd, buffer, 128)) != 0)
            {
                if (n == -1)
                    break;   // exit(1);

                ptr = &buffer[0];
                while (n > 0)
                {
                    if ((nw = write(newfd, ptr, n)) <= 0)
                        exit(1);
                    n -= nw;
                    ptr += nw;
                    printf("%s", buffer);
                }
                for (int i = 0; i < 128; i++)
                {
                    buffer[i] = '\0';
                }
                ptr = NULL;
            }
            close(newfd);
        }

        if (FD_ISSET(STDIN_FILENO, &readfds)) 
        {
            // Entrada padrão (stdin)
            printf("Data available on stdin...\n");
            // Faça algo com a entrada padrão
            fgets(buffer, sizeof(buffer), stdin);
            printf("You entered: %s", buffer);

            if (strcmp(buffer, "x\n") == 0)
                break;
        }
    }

    close(fd);

    freeaddrinfo(res);
    close(fd);
    exit(0);
}
