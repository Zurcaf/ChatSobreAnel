// test.c
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>

int main(void)
{
struct addrinfo hints,*res,*p;
int errcode;
char buffer[INET_ADDRSTRLEN];
struct in_addr *addr;

memset(&hints,0,sizeof hints);
hints.ai_family=AF_INET;//IPv4
hints.ai_socktype=SOCK_DGRAM;
hints.ai_flags=AI_CANONNAME;

if((errcode=getaddrinfo("tejo.tecnico.ulisboa.pt",NULL,&hints,&res))!=0)
    fprintf(stderr,"error: getaddrinfo: %s\n",gai_strerror(errcode));
else{
    printf("canonical hostname: %s\n",res->ai_canonname);

for(p=res;p!=NULL;p=p->ai_next)
{
    struct sockaddr_in *ip = (struct sockaddr_in *)p->ai_addr;
    addr = &(ip->sin_addr);

    printf("internet address: %s (%08lX)\n",
    inet_ntop(p->ai_family, addr, buffer, sizeof(buffer)),
    (long unsigned int)ntohl(addr->s_addr));
}

freeaddrinfo(res);
}
exit(0);
}

// #include <arpa/inet.h>
// const char *inet_ntop(int af, const void *src,char *dst, socklen_t size);


// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netdb.h>
// int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
// void freeaddrinfo(struct addrinfo *res);
// const char *gai_strerror(int errcode);


// struct addrinfo { // (item in a linked list)
// int ai_flags; // additional options
// int ai_family; // address family
// int ai_socktype; // socket type
// int ai_protocol; // protocol
// socklen_t ai_addrlen; // address length (bytes)
// struct sockaddr *ai_addr; // socket address
// char *ai_canonname; // canonical hostname
// struct addrinfo *ai_next; // next item
// };


// struct sockaddr_in {
// sa_family_t sin_family;// address family: AF_INET
// u_int16_t sin_port; // port in (16 bits)
// struct in_addr sin_addr; // internet address
// };


// struct in_addr{
// uint32_t s_addr; // 32 bits
// };


// #include <string.h>
// void *memset(void *s,int c,size_t n);