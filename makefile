##Definir o compilador e suas flags
CFLAGS= -Wall
CC = gcc

#Definir os diretorios
OBJ = obj
SRC = src
HEAD = hed

#Definir os arquivos
SRCS = $(wildcard $(SRC)/*.c)
OBJS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SRCS))


BINDIR = bin
BIN = $(BINDIR)/COR

all: $(BIN)

#Criar o executavel
$(BIN): $(OBJS) | $(BINDIR)
	$(CC) $(CFLAGS) $(OBJS) -o $@

#Compilar arquivo auxFunc.c
$(OBJ)/auxFunc.o: $(SRC)/auxFunc.c $(HEAD)/main.h | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

#Compilar os arquivos
$(OBJ)/%.o: $(SRC)/%.c $(HEAD)/%.h | $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@


#Criar os diretorios
$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -r $(BINDIR)
	rm -r $(OBJ)

test:
	$(BINDIR)/COR 127.0.0.1 58009

test1:
	$(BINDIR)/COR 127.0.0.1 58001

test2:
	$(BINDIR)/COR 127.0.0.1 58002
	
test3:
	$(BINDIR)/COR 127.0.0.1 58003

test4:
	$(BINDIR)/COR 127.0.0.1 58004

test5:
	$(BINDIR)/COR 127.0.0.1 58005


debug:
	gdb $(BINDIR)/COR 127.0.0.1 58002



valgrind:
	valgrind --leak-check=full $(BINDIR)/COR 127.0.0.1 58002

