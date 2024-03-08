// Hello message sent to tejo.tecnico.ulisboa.pt:58001
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(void)
{
int fd;

int errcode;
char buffer[128 + 1];

struct addrinfo hints, *res;
struct sockaddr addr;
socklen_t addrlen;
ssize_t n;


fd=socket(AF_INET,SOCK_DGRAM,0);
if (fd == -1) /*error*/
        exit(1);

memset(&hints,0,sizeof hints);
hints.ai_family=AF_INET;
hints.ai_socktype=SOCK_DGRAM;

errcode=getaddrinfo("tejo.tecnico.ulisboa.pt","58001",&hints,&res);
if (errcode != 0)
    { /*error*/
        printf("Error connecting");
        exit(1);
    }

n=sendto(fd,"Hello!\n",7,0,res->ai_addr,res->ai_addrlen);
if (n == -1)
    { /*error*/
        printf("Error messaging.");
        exit(1);
    }


freeaddrinfo(res);

addrlen = sizeof(addr);
n = recvfrom(fd, buffer, 128, 0, &addr, &addrlen);

if (n == -1) /*error*/
    exit(1);

buffer[n] = '\0';

printf("echo: %s\n", buffer);

exit(0);
}

