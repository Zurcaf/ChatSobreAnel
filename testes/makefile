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

2_1: $(SRC)/2_1UDPSocketSendTo.c
	gcc $(CFLAGS) $(SRC)/2_1UDPSocketSendTo.c -o $(BINDIR)/2_1UDPSocketSendTo

3: $(SRC)/3UDPRecvfrom.c
	gcc $(CFLAGS) $(SRC)/3UDPRecvfrom.c -o $(BINDIR)/3UDPRecvfrom

4: $(SRC)/4TCPSocketConnect.c
	gcc $(CFLAGS) $(SRC)/4TCPSocketConnect.c -o $(BINDIR)/4TCPSocketConnect

5: $(SRC)/5TCPWriteRead.c
	gcc $(CFLAGS) $(SRC)/5TCPWriteRead.c -o $(BINDIR)/5TCPWriteRead

10: $(SRC)/10TCPServer.c
	gcc $(CFLAGS) $(SRC)/10TCPServer.c -o $(BINDIR)/10TCPServer

11: $(SRC)/11TCPClient.c
	gcc $(CFLAGS) $(SRC)/11TCPClient.c -o $(BINDIR)/11TCPClient

12_0: $(SRC)/12_0UDPClient.c
	gcc $(CFLAGS) $(SRC)/12_0UDPClient.c -o $(BINDIR)/12_0UDPClient

12_1: $(SRC)/12_1UDPClient.c
	gcc $(CFLAGS) $(SRC)/12_1UDPClient.c -o $(BINDIR)/12_1UDPClient

12_2: $(SRC)/12_2UDPClient.c
	gcc $(CFLAGS) $(SRC)/12_2UDPClient.c -o $(BINDIR)/12_2UDPClient

12_3: $(SRC)/12_3UDPClient.c
	gcc $(CFLAGS) $(SRC)/12_3UDPClient.c -o $(BINDIR)/12_3UDPClient


13: $(SRC)/13toke.c
	gcc $(CFLAGS) $(SRC)/13toke.c -o $(BINDIR)/13toke

14: $(SRC)/14SelectedServer.c
	gcc $(CFLAGS) $(SRC)/14SelectedServer.c -o $(BINDIR)/14SelectedServer


0t: 0
	./$(BINDIR)/0getHostName

1t: 1
	./$(BINDIR)/1getAddrInfo

2_1t: 2_1
	./$(BINDIR)/2_1UDPSocketSendTo

2t: 2
	./$(BINDIR)/2UDPSocketSendTo

3t: 3
	./$(BINDIR)/3UDPRecvfrom

4t: 4
	./$(BINDIR)/4TCPSocketConnect

5t: 5
	./$(BINDIR)/5TCPWriteRead

10t: 10
	./$(BINDIR)/10TCPServer

11t: 11
	./$(BINDIR)/11TCPClient

12_0t: 12_0
	./$(BINDIR)/12_0UDPClient

12_1t: 12_1
	./$(BINDIR)/12_1UDPClient

12_2t: 12_2
	./$(BINDIR)/12_2UDPClient

12_3t: 12_3
	./$(BINDIR)/12_3UDPClient

13t: 13
	./$(BINDIR)/13toke

14t: 14
	./$(BINDIR)/14SelectedServer


clean:
	rm $(BINDIR)/*