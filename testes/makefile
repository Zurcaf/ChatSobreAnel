##Definir o compilador e suas flags
CFLAGS= -Wall -std=gnu99 -g
CC = gcc

#Definir os diretorios
OBJ = obj
SRC = src
BINDIR = bin
BIN = $(BINDIR)/COR


#Definir os arquivos
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))


0: $(SRC)/0getHostName.c
	gcc $(CFLAGS) $(SRC)/0getHostName.c -o $(BINDIR)/0getHostName

1: $(SRC)/1getAddrInfo.c
	gcc $(CFLAGS) $(SRC)/1getAddrInfo.c -o $(BINDIR)/1getAddrInfo

2: $(SRC)/2UDPSocketSendTo.c
	gcc $(CFLAGS) $(SRC)/2UDPSocketSendTo.c -o $(BINDIR)/2UDPSocketSendTo

3: $(SRC)/3UDPRecvfrom.c
	gcc $(CFLAGS) $(SRC)/3UDPRecvfrom.c -o $(BINDIR)/3UDPRecvfrom


#Testes de cada codigo
0t: 0
	./$(BINDIR)/0getHostName

1t: 1
	./$(BINDIR)/1getAddrInfo

2t: 2
	./$(BINDIR)/2UDPSocketSendTo

3t: 3
	./$(BINDIR)/3UDPRecvfrom


clean:
	rm $(BINDIR)/0getHostName
	rm $(BINDIR)/1getAddrInfo
	rm $(BINDIR)/2UDPSocketSendTo
	rm $(BINDIR)/3UDPRecvfrom