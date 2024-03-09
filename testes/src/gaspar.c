#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>

#define Max 129
#define MAX_NODES 16

// GRUPO 112

typedef struct nodedetails
{
    // Node id
    unsigned int id;
    // TCP server's port
    int self_port;
    // Server IP
    char ip[INET_ADDRSTRLEN];
    // Server socket file descriptor
    int fd;
    // Predecessor's connection's socket file descriptor (-1 if a connection does not exist)
    int pred_fd;
    // Successor's connection's socket file descriptor (-1 if a connection does not exist)
    int suc_fd;
    // 2nd Successor's connection's socket file descriptor (-1 if a connection does not exist)
    int suc2_fd;
    // Socket file descriptor of a temporary connection (-1 if a connection does not exist)
    // int temp_fd;
    // Connection information pertaining to this node's predecessor (NULL if a connection does not exist)
    // t_conn_info *predecessor;
    // Connection information pertaining to this node's successor (NULL if a connection does not exist)
    // t_conn_info *successor;
    // Temporary connection information (NULL if a connection does not exist)
    // t_conn_info *temp;
    // Predecessor IP address
    char pred_ip[INET_ADDRSTRLEN];
    // Successor IP address
    char suc_ip[INET_ADDRSTRLEN];
    // 2nd Successor IP address
    char suc2_ip[INET_ADDRSTRLEN];
    // Predecessor port
    unsigned int pred_port;
    // Successor port
    unsigned int suc_port;
    // 2nd Successor port
    unsigned int suc2_port;
    // Predecessor ID
    unsigned int pred_id;
    // Successor ID
    unsigned int suc_id;
    // 2nd Successor ID
    unsigned int suc2_id;
    // Search requests
    // int requests[100];
    // Search request info
    // struct sockaddr request_addr[100];
    // socklen_t request_addr_len[100];
} nodedetails;

///////////////////////////////////7

typedef struct
{
    int ring_number;
    nodedetails nd[MAX_NODES];
    int node_count;
} NodeList;

void parse_nodes_list(const char *message, NodeList *nodeList)
{
    sscanf(message, "NODESLIST %d", &nodeList->ring_number);

    const char *line = strtok((char *)message, "\n"); // Skip the first line
    line = strtok(NULL, "\n");                        // Start from the second line

    nodeList->node_count = 0;
    while (line != NULL && nodeList->node_count < MAX_NODES)
    {
        sscanf(line, "%d %s %d",
               &nodeList->nd[nodeList->node_count].id,
               nodeList->nd[nodeList->node_count].ip,
               &nodeList->nd[nodeList->node_count].self_port);

        nodeList->node_count++;
        line = strtok(NULL, "\n");
    }
}

NodeList getallnodes(NodeList *nodelist)
{
    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;
    char buffer[1000];
    // NodeList nodelist;

    struct addrinfo hints, *res;
    int fd, errcode;
    fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket

    if (fd == -1) /*error*/
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
    // n = sendto(fd, "UNREG 112 01", 32, 0, res->ai_addr, res->ai_addrlen);// UNREG 112 05   ///NODES 112
    n = sendto(fd, "NODES 112 ", 32, 0, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    { /*error*/
        printf("Error messaging.");
        exit(1);
    }

    freeaddrinfo(res);
    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 1000, 0, &addr, &addrlen);

    if (n == -1) /*error*/
        exit(1);

    buffer[n] = '\0';

    // printf("echo: %s\n", buffer);
    close(fd);
    parse_nodes_list(buffer, nodelist);
    if(nodelist->node_count!=0){
    printf("first node: %02d %s %d\n",
           nodelist[0].nd[0].id,
           nodelist[0].nd[0].ip,
           nodelist[0].nd[0].self_port);
    }
if(nodelist->node_count > 1){
    printf("last node: %02d %s %d\n",
           nodelist[0].nd[nodelist->node_count - 1].id,
           nodelist[0].nd[nodelist->node_count - 1].ip,
           nodelist[0].nd[nodelist->node_count - 1].self_port);
           }
    return *nodelist;
}

////////////////////////////////////

int check_ip(const char *ip)
{
    char *endptr;
    long octet;
    int count = 0;

    // Usando strtok para dividir o IP em octetos
    char *token = strtok(strdup(ip), ".");

    while (token != NULL)
    {
        octet = strtol(token, &endptr, 10);

        // Verifica se a conversão foi bem-sucedida e se o octeto está no intervalo válido (0 a 255)
        if (*endptr != '\0' || octet < 0 || octet > 255)
        {
            return 0; // Retorna 0 se o IP for inválido
        }

        token = strtok(NULL, ".");
        count++;
    }

    // Um IP válido deve ter 4 octetos
    return count == 4;
}

int countWords(char *line)
{
    int count = 0;
    bool inWord = false;

    while (*line)
    {
        if (*line == ' ' || *line == '\t' || *line == '\n')
        {
            // Whitespace character, transition out of a word
            inWord = false;
        }
        else
        {
            // Non-whitespace character, transition into a word
            if (!inWord)
            {
                count++;
                inWord = true;
            }
        }

        line++;
    }

    return count;
}

/////////////////////////////////////////////////////////////////////
int join(int ring, char ip[16], int id, int tcpport)
{
    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;
    char buffer[128 + 1];
    int nchars, i;

    struct addrinfo hints, *res;
    int fd, errcode;
    fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket

    if (fd == -1) /*error*/
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

    for (i = 0; i < 129; i++)
    {
        buffer[i] = 0;
    }

    printf("Connected\n");
    printf("%d %d %s %d\n", ring, id, ip, tcpport);
    sprintf(buffer, "REG %03d %02d %s %d", ring, id, ip, tcpport);

    nchars = strlen(buffer);
    printf("%s\n", buffer);
    n = sendto(fd, buffer, nchars, 0, res->ai_addr, res->ai_addrlen);

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
    close(fd);
    return 0;
}

////////////////////////////////////////////////////////////////////////
int leave(int ring, int id)
{
    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n;
    char buffer[128 + 1];
    int nchars, i;

    struct addrinfo hints, *res;
    int fd, errcode;
    fd = socket(AF_INET, SOCK_DGRAM, 0); // UDP socket

    if (fd == -1) /*error*/
        return -1;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;      // IPv4
    hints.ai_socktype = SOCK_DGRAM; // UDP socket
    errcode = getaddrinfo("tejo.tecnico.ulisboa.pt", "59000", &hints, &res);

    if (errcode != 0)
    { /*error*/
        printf("Error connecting");
        return -1;
    }

    for (i = 0; i < 129; i++)
    {
        buffer[i] = 0;
    }

    printf("Connected\n");
    printf("%d %d\n", ring, id);
    if (id < 10 && id >= 0)
    {
        sprintf(buffer, "UNREG %d 0%d", ring, id);
    }
    else if (9 < id < 100)
    {
        sprintf(buffer, "UNREG %d %d", ring, id);
    }
    else
    {
        return -1;
    }

    nchars = strlen(buffer);
    printf("%s\n", buffer);
    n = sendto(fd, buffer, nchars, 0, res->ai_addr, res->ai_addrlen);

    if (n == -1)
    { /*error*/
        printf("Error messaging.");
        return -1;
    }

    freeaddrinfo(res);
    addrlen = sizeof(addr);
    n = recvfrom(fd, buffer, 128, 0, &addr, &addrlen);

    if (n == -1) /*error*/
        return -1;

    buffer[n] = '\0';

    printf("echo: %s\n", buffer);
    close(fd);
    return 0;
}
/////////////////////////////////////////////////////////////////////////
void clean(char userInput[Max])
{
    int i;
    for (i = 0; i < 129; i++) // clean input
    {
        userInput[i] = 0;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

int msgTCP(char cmd[129], char *destIp, int tcp_port)
{
    ssize_t nbytes, nleft, nwritten, nread;
    char *ptr, buffer[128 + 1], buffer2[128];
    struct addrinfo hints, *res;
    int fd, n;
    fd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    if (fd == -1)
        return -1; // error
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_INET;       // IPv4
    hints.ai_socktype = SOCK_STREAM; // TCP socket
    // printf("dest port: %d\n", tcp_port);
    sprintf(buffer, "%d", tcp_port);
    printf("msg para: %s %s \n", destIp, buffer);
    // clean(buffer);
    n = getaddrinfo(destIp, buffer, &hints, &res);
    if (n != 0)
    { /*error*/
        printf("computador nao encontrado\n");
        return -1;
    }
    clean(buffer);

    n = connect(fd, res->ai_addr, res->ai_addrlen);
    if (n == -1)
    { /*error*/
        printf("nao ha conexao\n");
        return -1;
    }

    ptr = cmd;
    // ptr = strcpy(buffer, "Gosto de gamsa!\n");
    nbytes = 128;
    nleft = nbytes;
    while (nleft > 0)
    {
        nwritten = write(fd, ptr, nleft);
        if (nwritten <= 0) /*error*/
            return -1;
        nleft -= nwritten;
        ptr += nwritten;
    }

    nleft = nbytes;
    ptr = buffer;
    while (nleft > 0)
    {
        nread = read(fd, ptr, nleft);
        if (nread == -1) /*error*/
            return -1;
        else if (nread == 0)
            break; // closed by peer
        nleft -= nread;
        ptr += nread;
    }

    nread = nbytes - nleft;
    buffer[nread] = '\0';
    printf("echo: %s\n", buffer);
    close(fd);
    return 0;
}

int entry_prot(char *destIp, char *senderIp, int senderport, int destport, int id)
{
    char buffer[129];
    // printf("dest port: %d\n", destport);

    sprintf(buffer, "ENTRY %02d %s %d", id, senderIp, senderport);
    if (msgTCP(buffer, destIp, destport) == 0)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/////////////////////////////////////////////////////////////////////////

int show_topology(nodedetails nd)
{
    printf("Current Node \n ID: %d \n IP: %s \n", nd.id, nd.ip);
    printf("Predecessor \n ID: %d \n IP: %s \n", nd.pred_id, nd.pred_ip);
    printf("Successor \n ID: %d \n IP: %s \n", nd.suc_id, nd.suc_ip);
    printf("Second successor \n ID: %d \n IP: %s \n", nd.suc2_id, nd.suc2_ip);
    return 0;
}

//////////////////////////////////////////////////////////////////////////
int main(void)
{
    char COR[4], ip[16], RegIp[16], userInput[Max], RegIpaux[] = "193.136.138.142", cmd[3], straux[129], tcpchar[6], *ptr, buffer[129];
    int portTcp, regUDP, i, counter, maxfd, c;
    nodedetails nd;
    unsigned int ring = 4000, id = 200;
    NodeList nodelist;
    struct timeval tv;

    struct addrinfo hints, *res; ///////icializaçao de variáveis do server
    int fd, newfd, errcode;
    fd_set rfds;
    struct sockaddr addr;
    socklen_t addrlen;
    ssize_t n, nw;

    printf("Enter a line of text:");

    while (1)
    {
        fgets(userInput, sizeof(userInput), stdin);
        userInput[strcspn(userInput, "\n")] = '\0';

        int wordCount = countWords(userInput);

        printf("Number of words: %d\n", wordCount);

        if (wordCount == 3)
        {
            sscanf(userInput, "%s %s %d", COR, ip, &portTcp);

            if (strcmp("COR", COR) != 0)
            {
                printf("Not a valid command.\n");
                continue;
            }

            regUDP = 59000;
            strcpy(RegIp, RegIpaux);
            if (check_ip(ip))
            {
                strcpy(nd.ip, ip);
                printf("O IP %s está na forma correta.\n", ip);
            }
            else
            {
                printf("O IP %s não está na forma correta.\n", ip);
                printf("Not a valid command.\n");
                continue;
            }

            nd.self_port = portTcp;
            printf("tcp port: %d", nd.self_port);
        }
        else if (wordCount == 5)
        {
            sscanf(userInput, "%s %s %d %s %d", COR, ip, &portTcp, RegIp, &regUDP);

            if (strcmp("COR", COR) != 0)
            {
                printf("Not a valid command.\n");
                continue;
            }

            if (check_ip(RegIp))
            {
                printf("O RegIP %s está na forma correta.\n", RegIp);
            }
            else
            {
                printf("O RegIP %s não está na forma correta.\n", RegIp);
                printf("Not a valid command.\n");
                continue;
            }

            if (check_ip(ip))
            {
                strcpy(nd.ip, ip);
                printf("O IP %s está na forma correta.\n", ip);
            }
            else
            {
                printf("O IP %s não está na forma correta.\n", ip);
                printf("Not a valid command.\n");
                continue;
            }
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            nd.self_port = portTcp;
        }
        else
        {
            printf("Not a valid command.\n");
            continue;
        }

        // comand_list;
        printf("\nAvailable comands:\njoin (j) ring id\ndirect join (dj) id succid succIP succTCP\nchord (c)\nremove chord (rc)\nshow topology (st)\nshow routing (sr) dest\nshow path (sp) dest\nshow forwarding (sf)\nmessage (m) dest message\nleave (l)\nexit (x)\n");

        break;
    }

    nodelist = getallnodes(&nodelist);
    if (nodelist.node_count != 0)
    {
        printf("first node: %02d %s %d\n",
               nodelist.nd[0].id,
               nodelist.nd[0].ip,
               nodelist.nd[0].self_port);

        printf("last node: %02d %s %d\n",
               nodelist.nd[nodelist.node_count - 1].id,
               nodelist.nd[nodelist.node_count - 1].ip,
               nodelist.nd[nodelist.node_count - 1].self_port);
    }
    printf("total nodes: %d\n", nodelist.node_count);

    for (i = 0; i < 129; i++) // clean input
    {
        userInput[i] = 0;
    }
    /////////////////////////////////////////////////////// primeiro nó
    if (nodelist.node_count == 0)
    {

        while (1)
        {

            // scanf("%s", userInput);
            // userInput[strcspn(userInput, "\n")] = '\0';
            cmd[0] = 0;
            cmd[1] = 0;
            cmd[2] = 0;

            scanf("%s", cmd);
            if (strcmp(cmd, "j") == 0)
            {
                scanf("%d %d", &ring, &id); // auxint1: ring auxint2:id do no
                if (id > 99 || ring > 999)
                {
                    continue;
                }

                printf("j %d %d\n", ring, id);

                join(ring, ip, id, portTcp);
                nd.id = id;
                nd.suc_id=id;
                nd.suc2_id=id;
                nd.pred_id=id;
                nd.suc_fd=nd.self_port;
                nd.suc2_fd=nd.self_port;
                nd.pred_fd=nd.self_port;
                strcpy(nd.suc_ip,nd.ip);
                strcpy(nd.suc2_ip,nd.ip);
                strcpy(nd.pred_ip,nd.ip);
                printf("hello\n");
                sprintf(tcpchar, "%d", portTcp);

                if ((nd.fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
                    exit(1); // error
                memset(&hints, 0, sizeof hints);
                hints.ai_family = AF_INET;       // IPv4
                hints.ai_socktype = SOCK_STREAM; // TCP socket
                hints.ai_flags = AI_PASSIVE;
                if ((errcode = getaddrinfo(nd.ip, tcpchar, &hints, &res)) != 0) /*error*/
                    exit(1);
                if (bind(nd.fd, res->ai_addr, res->ai_addrlen) == -1)
                {
                    printf("error binding");
                    leave(ring, nd.id);
                    exit(1);
                }

                if (listen(nd.fd, 5) == -1)
                { /*error*/
                    printf("error listening");
                    exit(1);
                }

                addrlen = sizeof(addr);
                printf("Waiting to accept connections \n");
                maxfd = nd.fd; /* fd é o maior dos descritores a monitorizar, dado que o descritor do stdin é 0 (menor que fd) */
                break;
            }
            else if (strcmp(cmd, "l") == 0)
            {
                leave(ring, nd.id);
                exit(0);
            }
        }
    }
    
    else //////////////////////////////////////// join a um ring com nós
    {

        while (1)
        {

            // scanf("%s", userInput);
            // userInput[strcspn(userInput, "\n")] = '\0';
            cmd[0] = 0;
            cmd[1] = 0;
            cmd[2] = 0;

            scanf("%s", cmd);
            if (strcmp(cmd, "j") == 0)
            {
                scanf("%d %d", &ring, &id); // auxint1: ring auxint2:id do no
                if (id > 99 || ring > 999)
                {
                    continue;
                }

                printf("j %d %d\n", ring, id);
                join(ring, ip, id, portTcp);
                nd.id = id;
                printf("iniciando protocolo de entrada\n dest port:%d\n", nodelist.nd[0].self_port);

                entry_prot(nodelist.nd[0].ip, nd.ip, nd.self_port, nodelist.nd[0].self_port, nd.id);
                //printf("node id %d\n", nd.id);
                printf("hello\n");
                sprintf(tcpchar, "%d", portTcp);

                if ((nd.fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
                {
                    printf("error creating socket\n");
                    leave(ring, nd.id);
                    exit(1); // error
                }
                memset(&hints, 0, sizeof hints);
                hints.ai_family = AF_INET;       // IPv4
                hints.ai_socktype = SOCK_STREAM; // TCP socket
                hints.ai_flags = AI_PASSIVE;
                if ((errcode = getaddrinfo(nd.ip, tcpchar, &hints, &res)) != 0)
                { /*error*/
                    printf("error creating getaddrinfo\n");
                    leave(ring, nd.id);
                    exit(1);
                }
                if (bind(nd.fd, res->ai_addr, res->ai_addrlen) == -1)
                {
                    printf("error binding\n");
                    leave(ring, nd.id);
                    exit(1);
                }

                if (listen(nd.fd, 5) == -1)
                { /*error*/
                    printf("error listening\n");
                    exit(1);
                }

                addrlen = sizeof(addr);
                printf("Waiting to accept connections \n");

                maxfd = nd.fd; /* fd é o maior dos descritores a monitorizar, dado que o descritor do stdin é 0 (menor que fd) */

                if(nodelist.node_count==1){
                printf("tou aqui");
                nd.id = id;
                nd.suc_id=nodelist.nd[0].id;
                nd.suc2_id=id;
                nd.pred_id=nodelist.nd[0].id;
                nd.suc_fd=nodelist.nd[0].self_port;
                nd.suc2_fd=nd.self_port;
                nd.pred_fd=nodelist.nd[0].self_port;
                strcpy(nd.suc_ip,nodelist.nd[0].ip);
                strcpy(nd.suc2_ip,nd.ip);
                strcpy(nd.pred_ip,nodelist.nd[0].ip);

            }

                break;
            }
            else if (strcmp(cmd, "l") == 0)
            {
                leave(ring, nd.id);
                exit(0);
            }

        }
        // printf("iniciando protocolo de entrada\n dest port:%d\n", nodelist.nd[0].self_port);

        // entry_prot(nodelist.nd[0].ip, nd.ip, nd.fd, nodelist.nd[0].self_port, nd.id);
    }

    while (1)
    {
        printf("select\n");
        FD_ZERO(&rfds); /* remover todos os descritores do conjunto */
        FD_SET(nd.fd, &rfds);
        FD_SET(0, &rfds);
        tv.tv_sec = 0; /* 2 segundos, como exemplo */
        tv.tv_usec = 500;
        counter = 0;
        counter = select(maxfd + 1, &rfds, (fd_set *)NULL, (fd_set *)NULL, (struct timeval *)NULL);
        cmd[0] = 0;
        cmd[1] = 0;
        cmd[2] = 0;

        if (FD_ISSET(0, &rfds) != 0) /////////////////////////////////ler da consola
        {
            clean(userInput);
            scanf("%s",cmd);
            printf("user: %s\n", cmd);
            if (strcmp(cmd, "l") == 0)
            {
                leave(ring, nd.id);
                freeaddrinfo(res);
                close(fd);
                exit(0);
            }
            else if((strcmp(cmd, "st") == 0)){
                show_topology(nd);
            }

            continue;
        }
        else if (FD_ISSET(nd.fd, &rfds) != 0)
        {
            printf("got'hem\n");
            clean(userInput);

            addrlen = sizeof(addr);
            printf("Waiting to accept connections \n");
            if ((newfd = accept(nd.fd, &addr, &addrlen)) == -1)
                exit(1); /*error*/

            printf("Connection accepted\n");
            while ((n = read(newfd, userInput, 128)) != 0)
            {
                if (n == -1) /*error*/
                    break;   // exit(1);
                ptr = &userInput[0];
                printf("%s \n", userInput);

                sscanf(userInput,"%s",buffer);
                if(strcmp("ENTRY",buffer)==0){
                    sscanf(userInput,"%s %d %s %d",buffer, &nd.suc_id, nd.suc_ip, &nd.suc_fd);
                    nd.pred_fd=nd.suc_fd;
                    nd.pred_id= nd.suc_id;
                    strcpy(nd.pred_ip,nd.suc_ip);
                    clean(buffer);

                }
                while (n > 0)
                {
                    if ((nw = write(newfd, ptr, n)) <= 0) /*error*/
                        exit(1);
                    n -= nw;
                    ptr += nw;
                    printf("%s \n", userInput);
                }
            }
            close(newfd);

            // break;
        }
    }

    // fgets(userInput, sizeof(userInput), stdin);
    printf("Obrigado por senhor.\n");
    // exit(1);
}